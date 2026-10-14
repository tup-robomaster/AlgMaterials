// 练习 3-1 的答案：把 3.1 那份作业里的第三件单独抄出来。
//
// 第三件在原文里是第 33 行到第 43 行。抄出来之后，它自己就是整个程序。

#include <cstdio>

int main() {
    int i = 1;
    int j = 1;

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
