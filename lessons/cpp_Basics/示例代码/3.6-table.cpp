// 3.6：2.6 那张乘法表，拆成函数。
//
// 对照 示例代码/2.6-example.cpp 看——那里是一个 main 装两层循环。
// 这里把它剪成两块，各起一个名字：
//
//   printRow(i)    打第 i 行的 9 个格子
//   printTable()   把 9 行都打出来
//
// 算式、%2d、两个空格，一个字没改。拆函数不改结果，
// 这一点下面打出来的东西会证明：和 2.6 那张表一模一样。
//
// 跑一下：g++ -std=c++17 -Wall 示例代码/3.6-table.cpp -o /tmp/t && /tmp/t

#include <cstdio>

// 打一行：第 i 行，从 1*1 打到 i*i
void printRow(int i) {
    for (int j = 1; j <= i; j = j + 1) {
        printf("%d×%d=%2d ", j, i, i * j);
    }
    printf("\n");
}

// 打整张表：把 1 到 9 行都交给 printRow
void printTable() {
    for (int i = 1; i <= 9; i = i + 1) {
        printRow(i);
    }
}

int main() {
    printTable();
    return 0;
}
