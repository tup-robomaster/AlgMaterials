// 练习 2-26（找错）：while 版的乘法表，打出来只有一条斜线。
//
// 这文件里是**错的**那一版：内圈的 j 建到外圈外面去了。
// 结果是 j 不重新数——外圈转第二圈时 j 还停在上次结束的地方，
// 内圈只走得动一次，每一行只剩一个 i*i。

#include <cstdio>

int main() {
    int i = 1;
    int j = 1;

    while (i <= 9) {
        while (j <= i) {
            printf("%d×%d=%2d ", j, i, i * j);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }

    return 0;
}
