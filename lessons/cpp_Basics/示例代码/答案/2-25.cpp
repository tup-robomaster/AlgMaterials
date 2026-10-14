// 练习 2-25 答案：把乘法表倒过来打——第一行 9 个，最后一行 1 个。
//
// 和例题只差外圈：起点 9、步子 -1。内圈还是 j <= i。

#include <cstdio>

int main() {
    for (int i = 9; i >= 1; i = i - 1) {
        for (int j = 1; j <= i; j = j + 1) {
            printf("%d×%d=%2d ", j, i, i * j);
        }
        printf("\n");
    }

    return 0;
}
