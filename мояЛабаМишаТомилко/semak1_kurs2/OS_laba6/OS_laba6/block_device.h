#pragma once
#include <vector>
#include <cstdint>

constexpr uint32_t BLOCK_SIZE = 512;
constexpr uint32_t BLOCK_COUNT = 4096;

class BlockDevice {
public:
    BlockDevice();

    int allocateBlock();
    void freeBlock(int index);

    void readBlock(int index, uint8_t* buffer);
    void writeBlock(int index, const uint8_t* buffer);

private:
    std::vector<std::vector<uint8_t>> blocks;
    std::vector<bool> used;
};
