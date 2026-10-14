// 练习 1-3 的答案：三行赋值。
//
// 一步步走：
//     a = b;   a 变成 2      （b 还是 2）
//     b = c;   b 变成 3      （c 还是 3）
//     c = a;   c 变成 2      （a 已经是 2 了，不是 1）
// 最后一步最容易错——a 早就不是原来那个 a 了。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;
    int c = 3;

    a = b;
    b = c;
    c = a;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
