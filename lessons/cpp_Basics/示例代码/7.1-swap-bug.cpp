// 练习 7-4 正文里贴的那一份（错）：交换没用第三个盒子。
// 该打「a = 2，b = 1」，它打「a = 2，b = 2」。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;

    a = b;
    b = a;

    std::cout << "a = " << a << "，b = " << b << std::endl;

    return 0;
}
