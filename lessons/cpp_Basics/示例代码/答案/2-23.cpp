// 练习 2-23 答案：读 n，打 n 行的乘法表（不是固定的 9 行）。
//
// 和例题唯一的差别：外圈的上限从写死的 9 换成读进来的 n。
// 内圈一个字都没动。

#include <cstdio>

int main() {
    int n = 0;
    scanf("%d", &n);

    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            printf("%d×%d=%2d ", j, i, i * j);
        }
        printf("\n");
    }

    return 0;
}
