// 练习 2-24 答案：完整的 9×9 矩形乘法表——每一行都打满 9 个。
//
// 和例题差在内圈的上限：例题是 j <= i（一行比一行长），
// 这一题是 j <= 9（每行一样长）。

#include <cstdio>

int main() {
    for (int i = 1; i <= 9; i = i + 1) {
        for (int j = 1; j <= 9; j = j + 1) {
            printf("%d×%d=%2d ", j, i, i * j);
        }
        printf("\n");
    }

    return 0;
}
