// 3.5：3.4-swap-fail.cpp 的那个交换，加上 & 就对了。
//
// 函数体一个字没改：还是 t、还是三行、还是同样的顺序。
// 变的只有参数表——两个 int 后面各加了一个 &。
//
// 跟 3.4-swap-fail.cpp 摆在一起跑一遍，是这一课最有用的一个练习。

#include <iostream>

// 交换两个数——这次加上了 &
void swapTwo(int& x, int& y) {
    int t = x;
    x = y;
    y = t;
}

int main() {
    int a = 1;
    int b = 2;

    swapTwo(a, b);

    std::cout << a << " " << b << std::endl;
    return 0;
}
