// 1.2 类型：int、double、char、bool
//
// 四种最基本的类型各开一个盒子，看看它们分别装什么、打出来什么样。

#include <iostream>

int main() {
    int    n = 42;       // 整数
    double d = 3.14;     // 小数
    char   c = 'A';      // 一个字符
    bool   b = true;     // 真 / 假

    std::cout << n << std::endl;
    std::cout << d << std::endl;
    std::cout << c << std::endl;
    std::cout << b << std::endl;

    return 0;
}
