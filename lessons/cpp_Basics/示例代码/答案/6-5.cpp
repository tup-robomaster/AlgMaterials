// 练习 6-5 的答案：bump(r) 加的是 a
//
// r 是 a 的另一个名字，所以把 r 递进 bump 就等于把 a 递进去。
// b 从头到尾没人碰，还是 2。

#include <iostream>

void bump(int& x) {
    x = x + 1;
}

int main() {
    int a = 1;
    int b = 2;

    int& r = a;

    bump(r);

    std::cout << a << " " << b << std::endl;

    return 0;
}
