// 练习 3-11 的答案：两个参数都加 &。
//
// 只加一个不行——两个都要加。少加一个，那个就还是复印件，
// 换到一半就断了。

#include <iostream>

void swapTwo(int& x, int& y) {
    int t = x;
    x = y;
    y = t;
}

int main() {
    int a = 3;
    int b = 8;

    swapTwo(a, b);

    std::cout << a << " " << b << std::endl;
    return 0;
}
