#include <cstdio>

int main() {
    unsigned char bytes[5] = {0x00, 0x0F, 0x7F, 0x80, 0xFF};

    for (int i = 0; i < 5; i++) {
        printf("%3d  %02X\n", bytes[i], bytes[i]);
    }

    return 0;
}
