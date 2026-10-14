// 练习 3-13 的答案：3.1 那份作业，拆成三个函数。
//
// 对照 示例代码/3.1-bad-main.cpp 看——那边是一个 main 里三件事，
// 那三个 i = 1 的重置就是件与件之间在抢同一个盒子。
// 拆开之后，三个循环各用自己的 i，谁也不碍谁，重置那几行自然就没了。
//
// 这是第 3 部分的收尾题：3.1 的毛病，你现在能自己治了。

#include <iostream>
#include <cstdio>

// 第一件：打一个星号直角三角形
void printTriangle() {
    for (int i = 1; i <= 4; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }
}

// 第二件：1 加到 100
void printSum() {
    int sum = 0;
    for (int i = 1; i <= 100; i = i + 1) {
        sum = sum + i;
    }
    std::cout << "1 到 100 是 " << sum << std::endl;
}

// 第三件：9×9 乘法表
void printTable() {
    for (int i = 1; i <= 9; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            printf("%d×%d=%2d ", j, i, i * j);
        }
        printf("\n");
    }
}

int main() {
    printTriangle();
    std::cout << "----" << std::endl;

    printSum();
    std::cout << "----" << std::endl;

    printTable();

    return 0;
}
