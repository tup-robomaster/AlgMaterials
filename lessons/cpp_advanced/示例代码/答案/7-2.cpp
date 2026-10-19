#include <cstdio>
#include <iostream>
#include <vector>

int main() {
    std::vector<unsigned char> bytes;
    int value = 0;

    while (std::cin >> std::hex >> value) {
        bytes.push_back((unsigned char)value);
    }

    printf("共 %d 个字节\n", (int)bytes.size());
    for (int i = 0; i < (int)bytes.size(); i++) {
        printf(i == 0 ? "%d" : " %d", bytes[i]);
    }
    printf("\n");

    return 0;
}
