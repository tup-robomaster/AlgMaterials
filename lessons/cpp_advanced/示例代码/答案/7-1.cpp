#include <cstdio>
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        return 1;
    }

    unsigned char bytes[100] = {0};
    for (int i = 0; i < n; i++) {
        int value = 0;
        std::cin >> value;
        bytes[i] = (unsigned char)value;
    }

    printf("%d 个字节\n", n);
    for (int i = 0; i < n; i++) {
        printf(i == 0 ? "%02X" : " %02X", bytes[i]);
    }
    printf("\n");

    return 0;
}
