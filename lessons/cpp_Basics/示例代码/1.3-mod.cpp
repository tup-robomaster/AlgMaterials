// 1.3 运算 —— 补充：取余 % 能干三件很常用的事
//
// 取余就是「除完之后剩多少」。
// 7 % 2 = 1（7 除以 2 商 3 余 1）

#include <iostream>

int main() {
    int n = 1234;

    std::cout << n % 10 << std::endl;          // 个位
    std::cout << n / 10 % 10 << std::endl;     // 十位
    std::cout << n / 100 % 10 << std::endl;    // 百位
    std::cout << n % 2 << std::endl;           // 0 就是偶数，1 就是奇数

    return 0;
}
