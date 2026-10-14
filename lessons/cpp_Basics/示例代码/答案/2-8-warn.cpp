// 练习 2-8（找错）：想判断 a 是不是等于 5，结果判断的不是这回事。
//
// 编译器警告了一句，但它还是把程序编出来了，跑起来也是错的。

#include <iostream>

int main() {
    int a = 3;

    if (a = 5) {
        std::cout << "a 是 5" << std::endl;
    } else {
        std::cout << "a 不是 5" << std::endl;
    }

    std::cout << "a 现在是 " << a << std::endl;

    return 0;
}
