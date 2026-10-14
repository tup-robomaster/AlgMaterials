// 2.2 例题：判断一个数在不在 1 到 100 之间。
//
// 数学上会写成 1 <= x <= 100，在 C++ 里不能这么写，理由见课里。

#include <iostream>

int main() {
    int x = 42;

    if (x >= 1 && x <= 100) {
        std::cout << x << " 在范围里" << std::endl;
    } else {
        std::cout << x << " 不在范围里" << std::endl;
    }

    return 0;
}
