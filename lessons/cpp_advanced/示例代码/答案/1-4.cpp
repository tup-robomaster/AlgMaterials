// 练习 1-4：声明两遍、定义一遍。
//
// 编得过，还跑出 42——这就是 1.2 那个卫士真正在挡的东西：
// 它挡的是「定义」，声明贴几遍编译器一个字都不说。
//
// 声明和定义的区别就长在眼睛底下：
//   末尾一个分号、没有函数体  → 声明（承诺）
//   有个 { ... } 包着的函数体 → 定义（兑现）

#include <iostream>

int twice(int x);
int twice(int x);

int twice(int x) {
    return x * 2;
}

int main() {
    std::cout << twice(21) << std::endl;
    return 0;
}
