// 2.4：只写了两个格子，少一个分号。
//
// 头上一共三格、两个分号。少写一格，编译器看不懂。

#include <iostream>

int main() {
    for (int i = 1; i <= 5) {
        std::cout << i << std::endl;
    }

    return 0;
}
