// 练习 6-3 的答案：走 r 改 a
//
// r = 9; 写的不是 r 这个盒子（它压根不是盒子），是 a。
// 最后那行 1 是证据：它俩的地址一样，所以本来就是同一个盒子。

#include <iostream>

int main() {
    int a = 5;
    int& r = a;

    r = 9;

    std::cout << a << std::endl;
    std::cout << (&r == &a) << std::endl;

    return 0;
}
