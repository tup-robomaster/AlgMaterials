// 1.1 变量：给一个盒子起名字
// 这一课的主角。1.1 里的数据流动画用的就是这个程序。

#include <iostream>

int main() {
    int a = 3;
    int b = 5;
    int c = a + b;

    a = 10;              // 改 a，看看 c 会不会跟着变

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    return 0;
}
