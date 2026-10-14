// 2.6：同一张乘法表，用 while 再写一版。
//
// 和 2.6-example.cpp 打出来的东西一个字符都不差。
// 对照着看这两个文件，能看出 for 和 while 到底差在哪儿：
//
//   for 的三件事挤在头上，while 的三件事散在三处——
//   起点在进循环之前，条件在 while 后面，步子在循环体最后一行。
//
//   这里还多出一个坑：**内圈的 j 每一轮外圈都得重新置 1。**
//   for 版里这件事是自动的（int j = 1 就写在内圈头上，外圈每转一圈
//   都重新执行一遍）；while 版里不写 j = 1 那句，第二行就会少打几个——
//   因为 j 会停在上一次结束的地方。
//
// 跑一下：g++ -std=c++17 -Wall 示例代码/2.6-while.cpp -o /tmp/t && /tmp/t

#include <cstdio>

int main() {
    int i = 1;
    while (i <= 9) {
        int j = 1;                      // ← 这一句不能挪到循环外面去
        while (j <= i) {
            printf("%d×%d=%2d ", j, i, i * j);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }

    return 0;
}
