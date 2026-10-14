// 6.2 例题：一次改掉外面两个盒子。
//
// 两个参数都是引用，所以函数里的 x、y 就是 main 里 a、b 的另一个名字，
// 改它们就是改 a、b 本人。
// 值传递做不到这件事——一个函数只能 return 一样东西。

#include <iostream>

void timesTwo(int& x, int& y) {
    x = x * 2;
    y = y * 2;
}

int main() {
    int a = 3;
    int b = 5;

    timesTwo(a, b);

    std::cout << "a = " << a << "   b = " << b << std::endl;

    return 0;
}
