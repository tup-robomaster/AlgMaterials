#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <vector>

int main() {
    std::vector<unsigned char> data;
    char ch = 0;
    while (std::cin.get(ch)) {
        data.push_back((unsigned char)ch);
    }

    ssize_t written = 0;
    if (!data.empty()) {
        written = write(STDOUT_FILENO, &data[0], data.size());
    }
    printf("写入 %ld 字节\n", (long)written);

    return 0;
}
