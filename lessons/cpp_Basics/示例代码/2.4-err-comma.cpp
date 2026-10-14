// 2.4：两个分号写成逗号。
//
// 头上那三个格子是用分号隔开的，不是逗号。
// 写错一个字符，就是一场灾难——好在编译器会当场拦下来。

#include <iostream>

int main() {
    for (int i = 1, i <= 5; i = i + 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
