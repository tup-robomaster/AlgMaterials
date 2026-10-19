#include <cstdio>

int main() {
    char a = (char)0xFF;
    unsigned char b = 0xFF;

    printf("char: %d\n", a);
    printf("unsigned char: %d\n", b);
    printf("转成无符号之后: %d\n", (unsigned char)a);

    return 0;
}
