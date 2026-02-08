#include "filesystem.h"
#include <cstring>
#include <fstream>
#include <iostream>

FileSystem::FileSystem(BlockDevice& dev)
    : device(dev) {
    cwdBlock = device.allocateBlock(); 
}

uint32_t FileSystem::allocateChain(uint32_t bytes) {
    uint32_t blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    uint32_t first = -1, prev = -1;

    for (uint32_t i = 0; i < blocks; i++) {
        int b = device.allocateBlock();
        if (first == (uint32_t)-1) first = b;
        if (prev != (uint32_t)-1) {
            device.writeBlock(prev, (uint8_t*)&b);
        }
        prev = b;
    }
    return first;
}

bool FileSystem::createFile(const std::string& path) {
    DirEntry entry{};
    strncpy(entry.name, path.c_str(), 255);
    entry.isDir = false;
    entry.size = 0;
    entry.firstBlock = device.allocateBlock();

    uint8_t buf[BLOCK_SIZE];
    device.readBlock(cwdBlock, buf);
    memcpy(buf, &entry, sizeof(entry));
    device.writeBlock(cwdBlock, buf);
    return true;
}

FileHandle* FileSystem::openFile(const std::string& path) {
    uint8_t buf[BLOCK_SIZE];
    device.readBlock(cwdBlock, buf);
    DirEntry* e = (DirEntry*)buf;

    if (strcmp(e->name, path.c_str()) == 0) {
        auto* fh = new FileHandle;
        fh->firstBlock = e->firstBlock;
        fh->position = 0;
        return fh;
    }
    return nullptr;
}

void FileSystem::closeFile(FileHandle* fh) {
    delete fh;
}

int FileSystem::write(FileHandle* fh, const uint8_t* buffer, uint32_t size) {
    uint32_t block = fh->firstBlock;
    device.writeBlock(block, buffer);
    fh->position += size;
    return size;
}

int FileSystem::read(FileHandle* fh, uint8_t* buffer, uint32_t size) {
    uint32_t block = fh->firstBlock;
    device.readBlock(block, buffer);
    fh->position += size;
    return size;
}

void FileSystem::seek(FileHandle* fh, uint32_t pos) {
    fh->position = pos;
}

bool FileSystem::deleteFile(const std::string& path) {
    device.freeBlock(cwdBlock);
    return true;
}

bool FileSystem::mkdir(const std::string& path) {
    return createFile(path);
}

bool FileSystem::rmdir(const std::string& path) {
    return deleteFile(path);
}

bool FileSystem::chdir(const std::string& path) {
    return true;
}

std::vector<std::string> FileSystem::listDir(const std::string&) {
    return {"*.*"};
}

bool FileSystem::importFromHost(const std::string& host,
                                const std::string& fsFile) {
    std::ifstream f(host, std::ios::binary);
    if (!f) return false;

    createFile(fsFile);
    auto fh = openFile(fsFile);

    uint8_t buf[BLOCK_SIZE];
    while (!f.eof()) {
        f.read((char*)buf, BLOCK_SIZE);
        write(fh, buf, f.gcount());
    }

    closeFile(fh);
    return true;
}
