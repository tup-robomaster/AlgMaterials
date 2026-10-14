// 练习 6-4 的答案：一份改不动，一份改得动
//
// 两个函数体一模一样，都是 x = x + 1。
// 差的是参数表：int x 收的是抄件，int& x 收的是本人。
// 所以第一次打完还是 3，第二次打完变成 4。

#include <iostream>

void addOneBad(int x) {
    x = x + 1;
}

void addOne(int& x) {
    x = x + 1;
}

int main() {
    int a = 3;

    addOneBad(a);
    std::cout << a << std::endl;

    addOne(a);
    std::cout << a << std::endl;

    return 0;
}
