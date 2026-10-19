#include <cstdio>

int main() {
    unsigned int x = 0;
    if (scanf("%u", &x) != 1) {
        return 1;
    }

    int count = 0;
    unsigned int t = x;
    while (t != 0) {
        if ((t & 1u) != 0) {
            count++;
        }
        t = t >> 1;
    }

    printf("%u 的二进制里有 %d 个 1\n", x, count);

    return 0;
}
