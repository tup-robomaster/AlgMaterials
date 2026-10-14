// 练习 6-1 的答案：double、int、char、bool 各占几格
//
// 数字是这台机器上的（gcc 13 / 64 位）。换台机器 char 和 bool 还是 1，
// int 多半还是 4，double 有可能是 4。

#include <iostream>

int main() {
    std::cout << sizeof(double) << " "
              << sizeof(int)    << " "
              << sizeof(char)   << " "
              << sizeof(bool)   << std::endl;

    return 0;
}
