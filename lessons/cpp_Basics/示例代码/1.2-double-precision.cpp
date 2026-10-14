// 1.2 类型 —— 补充：double 存不下所有小数
//
// 0.1 + 0.2 在数学上等于 0.3。但计算机里的小数是用二进制存的，
// 0.1 和 0.2 都存不准，加起来就不是 0.3。

#include <iostream>

int main() {
    double a = 0.1;
    double b = 0.2;

    std::cout << a + b << std::endl;          // 看着像 0.3，其实是 0.30000000000000004
    std::cout << (a + b == 0.3) << std::endl; // 和 0.3 比，一样吗？
    std::cout << (a + b > 0.3) << std::endl;  // 比 0.3 大吗？

    return 0;
}
