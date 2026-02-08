#include "filesystem.h"
#include <iostream>

int main() {
    BlockDevice dev;
    FileSystem fs(dev);

    fs.createFile("test.txt");
    auto f = fs.openFile("test.txt");

    const char msg[] = "Hello FS!";
    fs.write(f, (uint8_t*)msg, sizeof(msg));

    fs.seek(f, 0);
    char buf[32] = {};
    fs.read(f, (uint8_t*)buf, sizeof(msg));

    std::cout << buf << std::endl;

    fs.closeFile(f);
    return 0;
}
