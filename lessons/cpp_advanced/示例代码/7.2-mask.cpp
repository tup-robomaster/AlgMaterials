#include <cstdio>

int main() {
    unsigned char x = 0x00;

    x = x | 0x01;
    x = x | 0x08;
    printf("置位后: %02X\n", x);

    x = x & (unsigned char)~0x01;
    printf("清位后: %02X\n", x);

    x = x ^ 0x08;
    printf("翻转后: %02X\n", x);

    unsigned char m = x & 0x04;
    printf("第 2 位: %d\n", m != 0 ? 1 : 0);

    return 0;
}
