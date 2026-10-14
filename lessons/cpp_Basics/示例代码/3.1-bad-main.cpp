// 3.1 的反面例子：三道练习的答案，全粘在一个 main 里。
//
// 这不是编出来的。交上来的作业里最常见的就是这个样子——
// 每一段单看都对，跑起来也出得来东西。
// 问题不在对错，在于它们挤在一起。

#include <iostream>
#include <cstdio>

int main() {
    int i = 1;
    int j = 1;

    // ── 第一件：打一个星号直角三角形（2.5 的例题） ──
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

    // ── 第二件：1 加到 100（2.4 的例题） ──
    int sum = 0;
    i = 1;
    while (i <= 100) {
        sum = sum + i;
        i = i + 1;
    }
    std::cout << "1 到 100 是 " << sum << std::endl;

    std::cout << "----" << std::endl;

    // ── 第三件：9×9 乘法表（2.6 那道综合练习） ──
    i = 1;
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
