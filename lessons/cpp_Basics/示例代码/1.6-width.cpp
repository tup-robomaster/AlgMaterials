// 1.6 补充：宽度 %2d %4d %-4d
//
// 每行两头各加一个竖线，好看见补了几个空格。
// 最后两行是刻意留的：%2d 打 128 会怎样。

#include <cstdio>

int main() {
    printf("|%d|\n", 7);
    printf("|%2d|\n", 7);
    printf("|%4d|\n", 7);
    printf("|%-4d|\n", 7);

    printf("|%4d|\n", 128);
    printf("|%2d|\n", 128);

    return 0;
}
