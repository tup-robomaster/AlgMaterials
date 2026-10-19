#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
    for (int i = 0; i < 2; i++) {
        std::string path;
        if (!std::getline(std::cin, path)) {
            return 1;
        }

        int fd = open(path.c_str(), O_RDWR | O_NOCTTY);
        if (fd == -1) {
            printf("打开失败 %s\n", path.c_str());
        } else {
            printf("打开成功 %s\n", path.c_str());
            close(fd);
        }
    }

    return 0;
}
