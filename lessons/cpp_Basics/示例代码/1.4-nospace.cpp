// 1.4 补充：空格是你自己打的，cout 不会替你加
//
// 同一个 a 和同一个 b，三种打法。盯住数字中间。

#include <iostream>

int main() {
    int a = 3;
    int b = 5;

    std::cout << a << b << std::endl;              // 中间什么都没有
    std::cout << a << " " << b << std::endl;       // 自己插一个空格进去
    std::cout << "和是" << a + b << std::endl;

    return 0;
}
