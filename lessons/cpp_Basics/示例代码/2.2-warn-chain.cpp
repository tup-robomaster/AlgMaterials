// 2.2：数学里的连写不等式搬进 C++ 会怎样。
//
// 999 和 0 都明显不在 1 到 100 之间，可三个都打了「在范围里」。
// 编译器警告了两句，但它还是把程序编出来了。

#include <iostream>

int main() {
    int x = 42;

    if (1 <= x <= 100) {
        std::cout << "42：" << "在范围里" << std::endl;
    } else {
        std::cout << "42：" << "不在范围里" << std::endl;
    }

    x = 999;

    if (1 <= x <= 100) {
        std::cout << "999：" << "在范围里" << std::endl;
    } else {
        std::cout << "999：" << "不在范围里" << std::endl;
    }

    x = 0;

    if (1 <= x <= 100) {
        std::cout << "0：" << "在范围里" << std::endl;
    } else {
        std::cout << "0：" << "不在范围里" << std::endl;
    }

    return 0;
}
