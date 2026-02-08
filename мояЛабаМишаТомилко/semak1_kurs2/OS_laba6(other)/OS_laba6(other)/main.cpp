#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <random>
#include <sstream>      // <-- НУЖНО
#include <algorithm>    // <-- min/max
#include <cstdint>      // <-- uint32_t / uint64_t
using namespace std;

string diskFile = "disk.bin";

unsigned BLOCK_SIZE = 0;
unsigned TOTAL_BLOCKS = 0;

unsigned freeListHead = 1;     // блок, где хранится список свободных блоков (мы используем 1)
bool inTransaction = false;

vector<pair<unsigned, vector<char>>> writeCache;   // {blockNum, data}
vector<unsigned> freeListCache;                    // список свободных блоков в транзакции

void writeBlock(unsigned n, const vector<char>& data) {
    fstream f(diskFile, ios::in | ios::out | ios::binary);
    f.seekp((uint64_t)n * BLOCK_SIZE);
    f.write(data.data(), BLOCK_SIZE);
    f.close();
}

void readBlock(unsigned n, vector<char>& data) {
    data.resize(BLOCK_SIZE);
    fstream f(diskFile, ios::in | ios::out | ios::binary);
    f.seekg((uint64_t)n * BLOCK_SIZE);
    f.read(data.data(), BLOCK_SIZE);
    f.close();
}

void saveFreeList(const vector<unsigned>& list) {
    vector<char> block(BLOCK_SIZE, 0);
    uint32_t count = (uint32_t)list.size();
    memcpy(block.data(), &count, 4);
    uint32_t* arr = (uint32_t*)(block.data() + 4);
    for (uint32_t i = 0; i < count; ++i) arr[i] = list[i];
    writeBlock(freeListHead, block);
}

vector<unsigned> loadFreeList() {
    vector<unsigned> list;
    vector<char> block;
    readBlock(freeListHead, block);
    uint32_t count = 0;
    memcpy(&count, block.data(), 4);
    uint32_t* arr = (uint32_t*)(block.data() + 4);
    for (uint32_t i = 0; i < count; ++i) list.push_back(arr[i]);
    return list;
}

void initFS(unsigned blockSize, unsigned totalBlocks) {
    BLOCK_SIZE = blockSize;
    TOTAL_BLOCKS = totalBlocks;
    // create file of zeros
    fstream f(diskFile, ios::binary | ios::out | ios::trunc);
    vector<char> zero(blockSize, 0);
    for (unsigned i = 0; i < totalBlocks; ++i) f.write(zero.data(), blockSize);
    f.close();

    // build free list: blocks 2..TOTAL_BLOCKS-1 are free initially (reserve 0 for super, 1 for freeList pointer)
    vector<unsigned> freeBlocks;
    for (unsigned i = 2; i < TOTAL_BLOCKS; ++i) freeBlocks.push_back(i);
    saveFreeList(freeBlocks);

    cout << "Файловая система инициализирована: blockSize=" << BLOCK_SIZE
        << " totalBlocks=" << TOTAL_BLOCKS << " free=" << freeBlocks.size() << "\n";
}

vector<unsigned> allocBlocks(unsigned count) {
    vector<unsigned> freeBlocks = inTransaction ? freeListCache : loadFreeList();
    vector<unsigned> result;
    for (unsigned i = 0; i < count && !freeBlocks.empty(); ++i) {
        result.push_back(freeBlocks.back());
        freeBlocks.pop_back();
    }
    if (inTransaction) freeListCache = freeBlocks; else saveFreeList(freeBlocks);
    return result;
}

void freeBlocksF(const vector<unsigned>& blocks) {
    vector<unsigned> freeBlocks = inTransaction ? freeListCache : loadFreeList();
    for (unsigned b : blocks) freeBlocks.push_back(b);
    if (inTransaction) freeListCache = freeBlocks; else saveFreeList(freeBlocks);
}

vector<unsigned> writeData(const vector<char>& data, vector<unsigned> blocks = {}) {
    unsigned need = (data.size() + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocks.empty()) blocks = allocBlocks(need);
    // if blocks size < need, we write only to available blocks (caller must check)
    for (unsigned i = 0; i < blocks.size(); ++i) {
        vector<char> buf(BLOCK_SIZE, 0);
        unsigned offset = i * BLOCK_SIZE;
        unsigned take = 0;
        if (offset < data.size()) take = (unsigned)min<size_t>(data.size() - offset, BLOCK_SIZE);
        if (take > 0) memcpy(buf.data(), data.data() + offset, take);
        if (inTransaction) writeCache.push_back({ blocks[i], buf }); else writeBlock(blocks[i], buf);
    }
    return blocks;
}

vector<char> readData(const vector<unsigned>& blocks) {
    vector<char> out;
    out.reserve(blocks.size() * BLOCK_SIZE);
    for (unsigned b : blocks) {
        vector<char> buf;
        readBlock(b, buf);
        out.insert(out.end(), buf.begin(), buf.end());
    }
    return out;
}

void beginTransaction() {
    if (inTransaction) return;
    inTransaction = true;
    writeCache.clear();
    freeListCache = loadFreeList();
}

void commit() {
    if (!inTransaction) return;
    for (auto& p : writeCache) writeBlock(p.first, p.second);
    saveFreeList(freeListCache);
    inTransaction = false;
    writeCache.clear();
    freeListCache.clear();
}

void rollback() {
    if (!inTransaction) return;
    inTransaction = false;
    writeCache.clear();
    freeListCache.clear();
}

struct FileEntry {
    string name;                // имя файла (без пути)
    uint64_t size = 0;          // размер в байтах
    uint32_t readCount = 0;     // для демонстрации
    vector<unsigned> blocks;    // линейный список номеров блоков
};

// Directory: содержит подкаталоги и файлы. Имя "/" для корня.
struct Dir {
    string name;
    vector<Dir> subdirs;
    vector<FileEntry> files;
};

// Глобальные переменные каталога
Dir rootDir;
vector<string> cwdPath = { "/" }; // текущий каталог: путь как список имен; root = "/"

// Вспомогательные: найти директорию по пути; если createIfMissing true — создаёт
Dir* findDirByPath(const vector<string>& pathParts, bool createIfMissing = false) {
    Dir* cur = &rootDir;
    if (pathParts.size() == 1 && pathParts[0] == "/") return cur;
    for (size_t i = 1; i < pathParts.size(); ++i) {
        const string& part = pathParts[i];
        bool found = false;
        for (auto& sd : cur->subdirs) {
            if (sd.name == part) { cur = &sd; found = true; break; }
        }
        if (!found) {
            if (createIfMissing) {
                cur->subdirs.push_back(Dir{ part, {}, {} });
                cur = &cur->subdirs.back();
            }
            else return nullptr;
        }
    }
    return cur;
}

vector<string> splitPath(const string& path) {
    vector<string> parts;
    if (path.empty()) return parts;
    if (path[0] == '/') {
        parts.push_back("/");
    }
    else {
        parts = cwdPath;
    }
    string tmp;
    stringstream ss(path);
    while (getline(ss, tmp, '/')) {
        if (tmp.empty()) continue;
        if (tmp == ".") continue;
        if (tmp == "..") {
            if (parts.size() > 1) parts.pop_back();
            continue;
        }
        parts.push_back(tmp);
    }
    if (parts.empty()) parts.push_back("/");
    return parts;
}

// Сериализация каталога в бинарный буфер (простая)
void serializeDir(const Dir& d, vector<char>& out) {
    // format: name_len (4) name bytes | n_subdirs(4) (subdirs...) | n_files(4) (files...)
    auto put_u32 = [&](uint32_t v) { char b[4]; memcpy(b, &v, 4); out.insert(out.end(), b, b + 4); };
    auto put_u64 = [&](uint64_t v) { char b[8]; memcpy(b, &v, 8); out.insert(out.end(), b, b + 8); };
    auto put_str = [&](const string& s) { put_u32((uint32_t)s.size()); out.insert(out.end(), s.begin(), s.end()); };
    put_str(d.name);
    put_u32((uint32_t)d.subdirs.size());
    for (auto& sd : d.subdirs) serializeDir(sd, out);
    put_u32((uint32_t)d.files.size());
    for (auto& f : d.files) {
        put_str(f.name);
        put_u64(f.size);
        put_u32(f.readCount);
        put_u32((uint32_t)f.blocks.size());
        for (auto b : f.blocks) put_u32(b);
    }
}

// Десериализация (возвращает позицию после чтения)
size_t deserializeDir(const vector<char>& buf, size_t pos, Dir& d) {
    auto get_u32 = [&](uint32_t& v) { memcpy(&v, buf.data() + pos, 4); pos += 4; };
    auto get_u64 = [&](uint64_t& v) { memcpy(&v, buf.data() + pos, 8); pos += 8; };
    auto get_str = [&](string& s) { uint32_t len; get_u32(len); s.assign(buf.data() + pos, buf.data() + pos + len); pos += len; };
    get_str(d.name);
    uint32_t nsub = 0; get_u32(nsub);
    d.subdirs.clear();
    for (uint32_t i = 0;i < nsub;++i) { Dir sd; pos = deserializeDir(buf, pos, sd); d.subdirs.push_back(move(sd)); }
    uint32_t nfiles = 0; get_u32(nfiles);
    d.files.clear();
    for (uint32_t i = 0;i < nfiles;++i) {
        FileEntry f;
        get_str(f.name);
        get_u64(f.size);
        get_u32(f.readCount);
        uint32_t nb; get_u32(nb);
        f.blocks.resize(nb);
        for (uint32_t j = 0;j < nb;++j) { uint32_t b; get_u32(b); f.blocks[j] = b; }
        d.files.push_back(move(f));
    }
    return pos;
}

// Сохранение каталога в блочное пространство (перезаписывает прошлую копию)
vector<unsigned> catalogBlocks; // где хранится сериализованный каталог (список блоков)
void saveCatalog() {
    vector<char> buf;
    serializeDir(rootDir, buf);
    // записать данные в блоки (если уже есть blocks, перезаписать туда; иначе выделить)
    vector<unsigned> newBlocks = writeData(buf, catalogBlocks);
    // возможно writeData вернул меньше блоков, но обычно хватит
    if (!newBlocks.empty()) catalogBlocks = newBlocks;
    // также можно (и нужно) сохранить catalogBlocks somewhere persistent — но для простоты оставляем в памяти
    // В реальной реализации нужно записать указатель на catalogBlocks в фиксированное место (суперблок).
}

// Загрузка каталога: если catalogBlocks пуст — попробуем прочитать блок 2 как начальную точку
void loadCatalog() {
    // Если catalogBlocks пуст, пытаемся прочитать блок 2 (если он не нулевой)
    // Для простоты: попробуем прочитать block 2; если он пустой (all zeros) — root empty
    vector<char> possible;
    readBlock(2, possible);
    bool allzero = true; for (char c : possible) if (c != 0) { allzero = false; break; }
    if (allzero) {
        // empty catalog
        rootDir.name = "/";
        rootDir.subdirs.clear(); rootDir.files.clear();
        catalogBlocks.clear();
        return;
    }
    // else, we attempt to read starting at block 2 and treat it as catalogBlocks single-block storage
    // Simpler approach: we read all blocks from 2..end that are non-zero and assemble buffer until deserialize succeeds.
    vector<char> buf;
    for (unsigned b = 2; b < TOTAL_BLOCKS; ++b) {
        vector<char> tmp; readBlock(b, tmp);
        bool allz = true; for (char c : tmp) if (c != 0) { allz = false; break; }
        if (allz) break;
        buf.insert(buf.end(), tmp.begin(), tmp.end());
    }
    if (buf.empty()) {
        rootDir.name = "/"; rootDir.subdirs.clear(); rootDir.files.clear(); catalogBlocks.clear(); return;
    }
    // try to deserialize
    try {
        Dir d; size_t pos = deserializeDir(buf, 0, d);
        rootDir = move(d);
        // set catalogBlocks to single-block 2 (we don't track exact blocks here)
        catalogBlocks.clear();
        catalogBlocks.push_back(2);
    }
    catch (...) {
        // fallback
        rootDir.name = "/"; rootDir.subdirs.clear(); rootDir.files.clear(); catalogBlocks.clear();
    }
}

// ------------------------------- ОПЕРАЦИИ ФС (демонстрационные) ---------------------------------

// find file by path (absolute/relative), returns pointer to file entry and dir pointer
pair<FileEntry*, Dir*> findFileByPath(const string& path) {
    auto parts = splitPath(path);
    if (parts.empty()) return { nullptr,nullptr };
    string fname = parts.back();
    parts.pop_back();
    if (parts.empty()) parts.push_back("/");
    Dir* dir = findDirByPath(parts, false);
    if (!dir) return { nullptr,nullptr };
    for (auto& f : dir->files) if (f.name == fname) return { &f, dir };
    return { nullptr,nullptr };
}

// create directory
bool fs_create_dir(const string& path) {
    auto parts = splitPath(path);
    if (parts.empty()) return false;
    string dname = parts.back();
    parts.pop_back();
    if (parts.empty()) parts.push_back("/");
    Dir* parent = findDirByPath(parts, true);
    if (!parent) return false;
    // check exists
    for (auto& sd : parent->subdirs) if (sd.name == dname) return false;
    parent->subdirs.push_back(Dir{ dname, {}, {} });
    saveCatalog();
    return true;
}

// remove empty dir
bool fs_remove_dir(const string& path) {
    auto parts = splitPath(path);
    if (parts.size() <= 1) return false; // cannot remove root
    string dname = parts.back();
    parts.pop_back();
    Dir* parent = findDirByPath(parts, false);
    if (!parent) return false;
    for (size_t i = 0;i < parent->subdirs.size();++i) {
        if (parent->subdirs[i].name == dname) {
            if (!parent->subdirs[i].subdirs.empty() || !parent->subdirs[i].files.empty()) return false;
            parent->subdirs.erase(parent->subdirs.begin() + i);
            saveCatalog();
            return true;
        }
    }
    return false;
}

// change current dir
bool fs_change_dir(const string& path) {
    auto parts = splitPath(path);
    Dir* d = findDirByPath(parts, false);
    if (!d) return false;
    cwdPath = parts;
    return true;
}

// get cwd info
string fs_get_cwd() {
    string res;
    if (cwdPath.size() == 1 && cwdPath[0] == "/") return "/";
    for (size_t i = 1;i < cwdPath.size();++i) {
        res += "/" + cwdPath[i];
    }
    if (res.empty()) res = "/";
    return res;
}

// list files in dir (mask supports "*.*" or "*" simple)
vector<string> fs_list_dir(const string& path, const string& mask = "*.*") {
    auto parts = splitPath(path);
    Dir* d = findDirByPath(parts, false);
    vector<string> out;
    if (!d) return out;
    // files
    for (auto& f : d->files) out.push_back(f.name);
    // subdirs
    for (auto& sd : d->subdirs) out.push_back(sd.name + "/");
    return out;
}

// create new file (write data), store blocks list in file entry (linear list)
bool fs_create_file(const string& path, const vector<char>& data) {
    auto parts = splitPath(path);
    if (parts.empty()) return false;
    string fname = parts.back(); parts.pop_back();
    if (parts.empty()) parts.push_back("/");
    Dir* dir = findDirByPath(parts, true);
    if (!dir) return false;
    // check exists
    for (auto& f : dir->files) if (f.name == fname) return false;
    FileEntry fe; fe.name = fname; fe.size = data.size(); fe.readCount = 0;
    fe.blocks = writeData(data);
    dir->files.push_back(move(fe));
    saveCatalog();
    return true;
}

// delete file
bool fs_delete_file(const string& path) {
    auto parts = splitPath(path);
    string fname = parts.back(); parts.pop_back();
    if (parts.empty()) parts.push_back("/");
    Dir* dir = findDirByPath(parts, false);
    if (!dir) return false;
    for (size_t i = 0;i < dir->files.size();++i) {
        if (dir->files[i].name == fname) {
            freeBlocksF(dir->files[i].blocks);
            dir->files.erase(dir->files.begin() + i);
            saveCatalog();
            return true;
        }
    }
    return false;
}

// open file: return handle index; we store open files in vector
struct OpenFile { Dir* dir; FileEntry* file; uint64_t pos; bool used; };
vector<OpenFile> openFiles;

int fs_open(const string& path) {
    auto res = findFileByPath(path);
    if (!res.first) return -1;
    // create handle
    OpenFile h; h.dir = res.second; h.file = res.first; h.pos = 0; h.used = true;
    openFiles.push_back(h);
    return (int)openFiles.size() - 1;
}

void fs_close(int handle) {
    if (handle < 0 || handle >= openFiles.size()) return;
    openFiles[handle].used = false;
    openFiles[handle].dir = nullptr;
    openFiles[handle].file = nullptr;
}

// seek: set position
bool fs_seek(int handle, int64_t pos, int whence = 0) {
    if (handle < 0 || handle >= openFiles.size()) return false;
    if (!openFiles[handle].used) return false;
    uint64_t newpos = 0;
    if (whence == 0) newpos = pos;
    else if (whence == 1) newpos = (int64_t)openFiles[handle].pos + pos;
    else if (whence == 2) newpos = (int64_t)openFiles[handle].file->size + pos;
    if (newpos > openFiles[handle].file->size) return false;
    openFiles[handle].pos = newpos;
    return true;
}

// read from file starting at current position, up to length bytes; returns bytes read
vector<char> fs_read(int handle, size_t length) {
    vector<char> out;
    if (handle < 0 || handle >= openFiles.size()) return out;
    if (!openFiles[handle].used) return out;
    FileEntry* fe = openFiles[handle].file;
    uint64_t pos = openFiles[handle].pos;
    if (pos >= fe->size) return out;
    size_t toread = (size_t)min<uint64_t>(length, fe->size - pos);
    // determine starting block index and offset
    size_t startBlockIdx = pos / BLOCK_SIZE;
    size_t offsetInBlock = pos % BLOCK_SIZE;
    size_t remaining = toread;
    while (remaining > 0 && startBlockIdx < fe->blocks.size()) {
        unsigned blockNum = fe->blocks[startBlockIdx];
        vector<char> blockBuf; readBlock(blockNum, blockBuf);
        size_t canTake = min<size_t>(remaining, BLOCK_SIZE - offsetInBlock);
        out.insert(out.end(), blockBuf.begin() + offsetInBlock, blockBuf.begin() + offsetInBlock + canTake);
        remaining -= canTake;
        startBlockIdx++;
        offsetInBlock = 0;
    }
    openFiles[handle].pos += out.size();
    fe->readCount++;
    saveCatalog();
    return out;
}

// write at current position, may extend file by allocating blocks
// strategy: if writing in middle, we will read-modify-write affected blocks.
// For simplicity we will write sequentially: build new full data = existing prefix + new data + existing suffix, then rewrite file blocks.
// This is simpler but can be less efficient.
bool fs_write(int handle, const vector<char>& data) {
    if (handle < 0 || handle >= openFiles.size()) return false;
    if (!openFiles[handle].used) return false;
    FileEntry* fe = openFiles[handle].file;
    uint64_t pos = openFiles[handle].pos;
    // load whole file content
    vector<char> whole;
    if (fe->size > 0) {
        whole = readData(fe->blocks);
        whole.resize(fe->size); // trim pad bytes
    }
    // ensure whole size >= pos
    if (whole.size() < pos) whole.resize(pos, 0);
    // splice
    vector<char> newwhole;
    newwhole.reserve(whole.size() + data.size());
    newwhole.insert(newwhole.end(), whole.begin(), whole.begin() + pos);
    newwhole.insert(newwhole.end(), data.begin(), data.end());
    if (pos + data.size() < whole.size()) {
        newwhole.insert(newwhole.end(), whole.begin() + pos + data.size(), whole.end());
    }
    // free old blocks
    freeBlocksF(fe->blocks);
    fe->blocks.clear();
    // write newwhole to blocks
    fe->blocks = writeData(newwhole);
    fe->size = newwhole.size();
    openFiles[handle].pos = pos + data.size();
    saveCatalog();
    return true;
}

// import from host filesystem path (real path) into FS path
bool fs_import_host(const string& hostPath, const string& destPath) {
    ifstream in(hostPath, ios::binary);
    if (!in) return false;
    vector<char> buf((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    // if dest exists - overwrite
    auto parts = splitPath(destPath);
    string fname = parts.back(); parts.pop_back(); if (parts.empty()) parts.push_back("/");
    Dir* dir = findDirByPath(parts, true);
    if (!dir) return false;
    // remove existing file if any
    for (size_t i = 0;i < dir->files.size();++i) if (dir->files[i].name == fname) { freeBlocksF(dir->files[i].blocks); dir->files.erase(dir->files.begin() + i); break; }
    FileEntry fe; fe.name = fname; fe.size = buf.size(); fe.readCount = 0;
    fe.blocks = writeData(buf);
    dir->files.push_back(move(fe));
    saveCatalog();
    return true;
}

// Helper: print info about current directory and files
void fs_print_info_current_dir() {
    cout << "Current dir: " << fs_get_cwd() << "\n";
    auto parts = cwdPath;
    Dir* d = findDirByPath(parts, false);
    if (!d) { cout << "(not found)\n"; return; }
    cout << "Subdirs:\n";
    for (auto& sd : d->subdirs) cout << "  [D] " << sd.name << "\n";
    cout << "Files:\n";
    for (auto& f : d->files) cout << "  [F] " << f.name << " size=" << f.size << " blocks=" << f.blocks.size() << " reads=" << f.readCount << "\n";
}

// FS status info
void fs_print_status() {
    auto freeBlocks = loadFreeList();
    cout << "FS status: blockSize=" << BLOCK_SIZE << " total=" << TOTAL_BLOCKS << " free=" << freeBlocks.size()
        << " serviceBytes=" << (freeBlocks.size() + 1) * BLOCK_SIZE << " transCacheEntries=" << writeCache.size() << "\n";
}

bool removeEmptyDirectoriesRecursive(Dir* dir, Dir* parent = nullptr, size_t indexInParent = 0) {
    // Сначала рекурсивно обработаем все подкаталоги
    for (int i = static_cast<int>(dir->subdirs.size()) - 1; i >= 0; --i) {
        removeEmptyDirectoriesRecursive(&dir->subdirs[i], dir, i);
    }
    
    // Проверяем, пуст ли текущий каталог (нет файлов и подкаталогов)
    if (dir->files.empty() && dir->subdirs.empty()) {
        // Не удаляем корневой каталог
        if (parent != nullptr) {
            parent->subdirs.erase(parent->subdirs.begin() + indexInParent);
            return true;
        }
    }
    return false;
}

struct DirInfo {
    Dir* current;
    Dir* parent;
    size_t index;
};

// Функция для поиска и удаления всех пустых каталогов
int fs_remove_empty_dirs() {
    int removedCount = 0;
    
    // Начинаем с корневого каталога
    vector<DirInfo> toCheck;
    
    // Собираем все каталоги для проверки (не включая корень в список на удаление)
    function<void(Dir*, Dir*, size_t)> collectDirs;
    collectDirs = [&](Dir* current, Dir* parent, size_t index) {
        for (size_t i = 0; i < current->subdirs.size(); ++i) {
            toCheck.push_back({&current->subdirs[i], current, i});
            collectDirs(&current->subdirs[i], current, i);
        }
    };
    
    collectDirs(&rootDir, nullptr, 0);
    
    // Проверяем каталоги в обратном порядке (сначала самые глубокие)
    for (int i = static_cast<int>(toCheck.size()) - 1; i >= 0; --i) {
        DirInfo info = toCheck[i];
        
        // Проверяем, пуст ли каталог
        if (info.current->files.empty() && info.current->subdirs.empty()) {
            if (info.parent != nullptr) {
                info.parent->subdirs.erase(info.parent->subdirs.begin() + info.index);
                removedCount++;
                
                // Обновляем индексы в toCheck для оставшихся элементов
                for (auto& item : toCheck) {
                    if (item.parent == info.parent && item.index > info.index) {
                        item.index--;
                    }
                }
            }
        }
    }
    
    // Сохраняем изменения
    if (removedCount > 0) {
        saveCatalog();
    }
    
    return removedCount;
}

// Альтернативная версия: рекурсивный обход с удалением (более простая)
int fs_remove_empty_dirs_recursive() {
    int removedCount = 0;
    
    // Рекурсивная функция для обработки каталогов
    function<bool(Dir*, Dir*, size_t)> processDir;
    processDir = [&](Dir* current, Dir* parent, size_t index) -> bool {
        // Сначала обрабатываем все подкаталоги
        for (int i = static_cast<int>(current->subdirs.size()) - 1; i >= 0; --i) {
            if (processDir(&current->subdirs[i], current, i)) {
                // Если подкаталог был удален, уменьшаем i, так как размер вектора изменился
                i = min(i, static_cast<int>(current->subdirs.size()) - 1);
            }
        }
        
        // Проверяем, нужно ли удалить текущий каталог (кроме корня)
        if (parent != nullptr && current->files.empty() && current->subdirs.empty()) {
            parent->subdirs.erase(parent->subdirs.begin() + index);
            removedCount++;
            return true; // Каталог был удален
        }
        
        return false; // Каталог не был удален
    };
    
    // Обрабатываем подкаталоги корневого каталога
    for (int i = static_cast<int>(rootDir.subdirs.size()) - 1; i >= 0; --i) {
        if (processDir(&rootDir.subdirs[i], &rootDir, i)) {
            // Если каталог был удален, корректируем индекс
            i = min(i, static_cast<int>(rootDir.subdirs.size()) - 1);
        }
    }
    
    // Сохраняем изменения
    if (removedCount > 0) {
        saveCatalog();
    }
    
    return removedCount;
}

// Функция для поиска пустых каталогов без их удаления
vector<string> fs_find_empty_dirs() {
    vector<string> emptyDirs;
    
    // Рекурсивный обход каталогов
    function<void(Dir*, const string&)> findEmpty;
    findEmpty = [&](Dir* current, const string& path) {
        // Проверяем, пуст ли текущий каталог
        if (current->files.empty() && current->subdirs.empty()) {
            // Каталог пуст
            if (path != "/") { // Не включаем корневой каталог
                emptyDirs.push_back(path);
            }
        } else {
            // Каталог не пуст, проверяем подкаталоги
            for (auto& sd : current->subdirs) {
                string newPath = (path == "/") ? path + sd.name : path + "/" + sd.name;
                findEmpty(&sd, newPath);
            }
        }
    };
    
    findEmpty(&rootDir, "/");
    return emptyDirs;
}

// Еще более простая версия (без сложных структур)
int fs_remove_empty_dirs_simple() {
    int removedCount = 0;
    bool removedAny;
    
    do {
        removedAny = false;
        vector<tuple<Dir*, string>> dirsToCheck; // {directory pointer, path}
        dirsToCheck.push_back({&rootDir, "/"});
        
        // Собираем все каталоги
        for (size_t i = 0; i < dirsToCheck.size(); ++i) {
            Dir* current = get<0>(dirsToCheck[i]);
            for (auto& sd : current->subdirs) {
                string path = get<1>(dirsToCheck[i]) + (get<1>(dirsToCheck[i]) == "/" ? "" : "/") + sd.name;
                dirsToCheck.push_back({&sd, path});
            }
        }
        
        // Проверяем в обратном порядке (сначала глубокие)
        for (int i = static_cast<int>(dirsToCheck.size()) - 1; i >= 0; --i) {
            Dir* current = get<0>(dirsToCheck[i]);
            string path = get<1>(dirsToCheck[i]);
            
            // Пропускаем корневой каталог
            if (path == "/") continue;
            
            // Находим родительский каталог
            size_t lastSlash = path.find_last_of('/');
            string parentPath = path.substr(0, lastSlash);
            if (parentPath.empty()) parentPath = "/";
            string dirName = path.substr(lastSlash + 1);
            
            // Находим родительский каталог
            Dir* parent = findDirByPath(splitPath(parentPath), false);
            if (!parent) continue;
            
            // Проверяем, пуст ли текущий каталог
            if (current->files.empty() && current->subdirs.empty()) {
                // Удаляем каталог
                for (size_t j = 0; j < parent->subdirs.size(); ++j) {
                    if (parent->subdirs[j].name == dirName) {
                        parent->subdirs.erase(parent->subdirs.begin() + j);
                        removedCount++;
                        removedAny = true;
                        saveCatalog(); // Сохраняем после каждого удаления
                        break;
                    }
                }
            }
        }
    } while (removedAny); // Повторяем, пока есть что удалять
    
    return removedCount;
}
int main() {
    setlocale(LC_ALL, "Russian");
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    initFS(1024, 200);

    loadCatalog();
    if (rootDir.name.empty()) {
        rootDir.name = "/";
        rootDir.subdirs.clear();
        rootDir.files.clear();
        saveCatalog();
    }

    vector<string> dirNames = { "/dir1", "/dir2", "/dir3", "/dir4" };
    for (auto& d : dirNames) fs_create_dir(d);
    
    fs_create_dir("/dir1/empty1");
    fs_create_dir("/dir2/empty2");
    fs_create_dir("/dir2/empty2/nested_empty");
    fs_create_dir("/dir3/non_empty_dir");
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> sizeDist(500, 5000);

    for (int i = 0; i < 4; ++i) {
        size_t fsize = sizeDist(gen);
        vector<char> buf(fsize, 'A' + i);
        string fname = "/file" + to_string(i+1) + ".bin";
        
        if (i % 2 == 0) {
            fs_create_file("/dir1" + fname, buf);
            cout << "Создан файл /dir1" + fname + " размер=" + to_string(fsize) + "\n";
        } else {
            fs_create_file("/dir4" + fname, buf);
            cout << "Создан файл /dir4" + fname + " размер=" + to_string(fsize) + "\n";
        }
    }
    
    vector<char> buf(1000, 'X');
    fs_create_file("/dir3/non_empty_dir/test.bin", buf);

    cout << "\n=== Исходная структура каталогов ===\n";
    fs_change_dir("/");
    fs_print_info_current_dir();
    
    cout << "\n=== Поиск пустых каталогов ===\n";
    auto emptyDirs = fs_find_empty_dirs();
    if (emptyDirs.empty()) {
        cout << "Пустых каталогов не найдено.\n";
    } else {
        cout << "Найдено пустых каталогов: " << emptyDirs.size() << "\n";
        for (const auto& dir : emptyDirs) {
            cout << "  " << dir << "\n";
        }
    }
    
    cout << "\n=== Удаление пустых каталогов (рекурсивная версия) ===\n";
    int removed = fs_remove_empty_dirs_recursive();
    cout << "Удалено каталогов: " << removed << "\n";
    
    cout << "\n=== Структура после удаления ===\n";
    fs_change_dir("/");
    fs_print_info_current_dir();
    
    cout << "\n=== Проверка после удаления ===\n";
    emptyDirs = fs_find_empty_dirs();
    if (emptyDirs.empty()) {
        cout << "Пустых каталогов не осталось.\n";
    } else {
        cout << "Осталось пустых каталогов: " << emptyDirs.size() << "\n";
        for (const auto& dir : emptyDirs) {
            cout << "  " << dir << "\n";
        }
    }
    
    fs_create_dir("/new_empty1");
    fs_create_dir("/new_empty2/sub_empty");
    
    cout << "\n=== После создания новых пустых каталогов ===\n";
    emptyDirs = fs_find_empty_dirs();
    cout << "Найдено пустых каталогов: " << emptyDirs.size() << "\n";
    
    cout << "\n=== Удаление пустых каталогов (простая версия) ===\n";
    removed = fs_remove_empty_dirs_simple();
    cout << "Удалено каталогов: " << removed << "\n";
    
    fs_print_status();
    cout << "\nDemo завершен.\n";
    return 0;
}
