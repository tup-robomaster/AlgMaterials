// 练习 3-2 的答案：这份就是「少写一句」的那一份。
//
// 和 3.1-bad-main.cpp 只差一行——第三件开头那句 i = 1; 没了。
// 它编译干净、跑得通，前两件一字不差，只是乘法表整个不见了。
// 交上来的是它，答案也是它：这一题要看的不是「改对」，是「为什么」。

#include <iostream>
#include <cstdio>

int main() {
    int i = 1;
    int j = 1;

    // ── 第一件：打一个星号直角三角形 ──
    i = 1;
    while (i <= 4) {
        j = 1;
        while (j <= i) {
            std::cout << "*";
            j = j + 1;
        }
        std::cout << std::endl;
        i = i + 1;
    }

    std::cout << "----" << std::endl;

    // ── 第二件：1 加到 100 ──
    int sum = 0;
    i = 1;
    while (i <= 100) {
        sum = sum + i;
        i = i + 1;
    }
    std::cout << "1 到 100 是 " << sum << std::endl;

    std::cout << "----" << std::endl;

    // ── 第三件：9×9 乘法表（开头那句 i = 1; 抽掉了） ──
    while (i <= 9) {
        j = 1;
        while (j <= i) {
            printf("%d×%d=%2d ", j, i, i * j);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }

    return 0;
}
