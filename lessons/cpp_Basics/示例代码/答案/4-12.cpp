// 练习 4-12 的答案：在两行赋值前面加一行 int t = a[0];
//
// 不是删哪一行，是加一行。a[0] = a[1]; 一执行，原来那个 1 就被盖掉了，
// 所以盖之前得先把它存进 t。

#include <iostream>

int main() {
    int a[2] = {1, 2};

    int t = a[0];
    a[0] = a[1];
    a[1] = t;

    std::cout << a[0] << " " << a[1] << std::endl;

    return 0;
}
