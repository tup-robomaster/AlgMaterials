// 练习 7-4（找错）：交换没用第三个盒子。
//
// a = b; 跑完，a 原来那个 1 就没了；
// 第二句 b = a; 抄回来的其实是已经变成 2 的 a。
// 先拿一个 t 把 1 存起来，才换得成。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;
    int t = 0;

    t = a;
    a = b;
    b = t;

    std::cout << "a = " << a << "，b = " << b << std::endl;

    return 0;
}
