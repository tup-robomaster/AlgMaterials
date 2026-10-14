// 练习 3-12 的答案：矩形乘法表拆成两个函数。
//
// 和 示例代码/3.6-table.cpp 只差内圈那个条件：
//   三角版 j <= i     矩形版 j <= 9
// 「打一行」和「打整张表」这两件事没变，所以函数的骨架也不用变。

#include <cstdio>

void printRow(int i) {
    for (int j = 1; j <= 9; j = j + 1) {
        printf("%d×%d=%2d ", j, i, i * j);
    }
    printf("\n");
}

void printTable() {
    for (int i = 1; i <= 9; i = i + 1) {
        printRow(i);
    }
}

int main() {
    printTable();
    return 0;
}
