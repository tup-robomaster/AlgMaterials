// 练习 7-5（找错）：整数除法。
//
// 前三行都对，第一处不一样在第四行——这是 1.3 那个坑。
// 病根在第二行：b 是 int，两个整数相除，结果一定是整数，小数部分直接扔。
// 把 b 改成 double，四行全对。

#include <iostream>

int main() {
    int a = 7;
    double b = 2;

    std::cout << "和：" << a + b << std::endl;
    std::cout << "差：" << a - b << std::endl;
    std::cout << "积：" << a * b << std::endl;
    std::cout << "商：" << a / b << std::endl;

    return 0;
}
