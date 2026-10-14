// 6.1 每种类型占几格
//
// 这里的数字是这台机器上的结果，换台机器可能不一样
// （特别是 double 和指针，32 位机器上都是 4）。

#include <iostream>

int main() {
    std::cout << sizeof(char)   << " "
              << sizeof(bool)   << " "
              << sizeof(int)    << " "
              << sizeof(double) << std::endl;

    return 0;
}
