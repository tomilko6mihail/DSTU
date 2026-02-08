#pragma once
#include "block_device.h"
#include <string>
#include <vector>
#include <map>

struct DirEntry {
    char name[256];
    bool isDir;
    uint32_t firstBlock;
    uint32_t size;
};

struct FileHandle {
    uint32_t firstBlock;
    uint32_t position;
};

class FileSystem {
public:
    FileSystem(BlockDevice& dev);

    bool createFile(const std::string& path);
    FileHandle* openFile(const std::string& path);
    void closeFile(FileHandle* fh);

    int read(FileHandle* fh, uint8_t* buffer, uint32_t size);
    int write(FileHandle* fh, const uint8_t* buffer, uint32_t size);
    void seek(FileHandle* fh, uint32_t pos);

    bool deleteFile(const std::string& path);

    bool mkdir(const std::string& path);
    bool rmdir(const std::string& path);
    bool chdir(const std::string& path);
    std::vector<std::string> listDir(const std::string& path);

    bool importFromHost(const std::string& hostFile,
                        const std::string& fsFile);

private:
    BlockDevice& device;
    uint32_t cwdBlock;

    DirEntry* findEntry(const std::string& path, uint32_t& parent);
    uint32_t allocateChain(uint32_t bytes);
};
