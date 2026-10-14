// 2.1：括号里放的到底是什么。
//
// 0 是假，别的数都是真。比较算式算出来的也是这个。

#include <iostream>

int main() {
    if (0) {
        std::cout << "0 是真" << std::endl;
    }

    if (1) {
        std::cout << "1 是真" << std::endl;
    }

    if (-1) {
        std::cout << "-1 是真" << std::endl;
    }

    if (3 - 3) {
        std::cout << "3-3 是真" << std::endl;
    }

    if (10 > 3) {
        std::cout << "10>3 是真" << std::endl;
    }

    return 0;
}
