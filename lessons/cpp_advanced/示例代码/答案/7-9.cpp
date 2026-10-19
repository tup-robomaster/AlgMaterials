#include <cstdio>
#include <iostream>
#include <vector>

int main() {
    std::vector<unsigned char> bytes;
    int value = 0;

    while (std::cin >> std::hex >> value) {
        bytes.push_back((unsigned char)value);
    }

    int group = 1;
    for (int start = 0; start < (int)bytes.size(); start += 3) {
        int count = (int)bytes.size() - start;
        if (count > 3) {
            count = 3;
        }

        printf("第 %d 组 %d 字节:", group, count);
        for (int i = 0; i < count; i++) {
            printf(" %02X", bytes[start + i]);
        }
        printf("\n");
        group++;
    }

    return 0;
}
