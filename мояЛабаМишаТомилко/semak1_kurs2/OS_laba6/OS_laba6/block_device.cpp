#include "block_device.h"
#include <cstring>

BlockDevice::BlockDevice() {
    blocks.resize(BLOCK_COUNT, std::vector<uint8_t>(BLOCK_SIZE));
    used.resize(BLOCK_COUNT, false);
}

int BlockDevice::allocateBlock() {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (!used[i]) {
            used[i] = true;
            memset(blocks[i].data(), 0, BLOCK_SIZE);
            return i;
        }
    }
    return -1;
}

void BlockDevice::freeBlock(int index) {
    used[index] = false;
}

void BlockDevice::readBlock(int index, uint8_t* buffer) {
    memcpy(buffer, blocks[index].data(), BLOCK_SIZE);
}

void BlockDevice::writeBlock(int index, const uint8_t* buffer) {
    memcpy(blocks[index].data(), buffer, BLOCK_SIZE);
}
