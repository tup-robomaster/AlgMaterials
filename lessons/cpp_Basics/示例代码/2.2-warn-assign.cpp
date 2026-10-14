// 2.2：把 == 写成了 =。
//
// 这一份能编译通过，但编译器会警告你。跑出来的结果也是错的。

#include <iostream>

int main() {
    int a = 5;

    if (a = 0) {
        std::cout << "进来了" << std::endl;
    } else {
        std::cout << "没进来" << std::endl;
    }

    std::cout << "a 现在是 " << a << std::endl;

    return 0;
}
