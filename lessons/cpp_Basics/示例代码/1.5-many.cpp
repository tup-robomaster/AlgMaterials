// 1.5 补充：一次读三个，和 >> 串起来
//
// 两种读法做的是同一件事。输入的时候空格和回车都算数，
// 三个数敲在一行也行，一行一个也行。

#include <iostream>

int main() {
    int a;
    int b;
    int c;

    std::cin >> a >> b >> c;        // 一串 >> 读完三个

    std::cout << a + b + c << std::endl;

    return 0;
}
