#include <cstdio>

int main() {
    int high = 0;
    int low = 0;
    if (scanf("%d %d", &high, &low) != 2) {
        return 1;
    }

    unsigned char b = (unsigned char)((high << 4) | low);

    printf("拼成 0x%02X\n", b);
    printf("拆开 %d %d\n", (b >> 4) & 0x0F, b & 0x0F);

    return 0;
}
