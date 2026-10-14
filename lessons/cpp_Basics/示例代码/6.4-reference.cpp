// 6.4 引用传递：函数拿到的是本人
//
// 参数变成 int& x——x 就是 a 的另一个名字，没有抄件这回事。
// 调用那行还是 addOne(a)，跟值传递长得一模一样，所以最容易看漏。

#include <iostream>

void addOne(int& x) {
    x = x + 1;
}

int main() {
    int a = 3;

    addOne(a);

    std::cout << a << std::endl;

    return 0;
}
