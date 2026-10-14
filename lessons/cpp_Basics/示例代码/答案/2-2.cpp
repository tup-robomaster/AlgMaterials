// 练习 2-2：读两个整数，打较大的那个。
//
// 先把 a 当成答案，再看 b 是不是更大。b 更大就换成 b。

#include <iostream>

int main() {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;

    if (a >= b) {
        std::cout << a << std::endl;
    } else {
        std::cout << b << std::endl;
    }

    return 0;
}
