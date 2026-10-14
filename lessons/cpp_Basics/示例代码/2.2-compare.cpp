// 2.2：六个比较运算符各算一遍，把结果打出来。
//
// 结果只有 0 和 1——因为比出来的是真/假，而 cout 把真打成 1、假打成 0。
// 括号不能省，理由见课里。

#include <iostream>

int main() {
    int a = 5;
    int b = 3;

    std::cout << (a == b) << std::endl;
    std::cout << (a != b) << std::endl;
    std::cout << (a > b) << std::endl;
    std::cout << (a < b) << std::endl;
    std::cout << (a >= b) << std::endl;
    std::cout << (a <= b) << std::endl;

    return 0;
}
