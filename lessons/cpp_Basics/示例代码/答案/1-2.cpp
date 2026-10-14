// 练习 1-2 的答案：把 a 换成 100，别的不动。
//
// c 是在 a 变成 100 之后才算的，所以 c 跟着变。

#include <iostream>

int main() {
    int a = 100;
    int b = 20;
    int c = a + b;
    std::cout << c << std::endl;
    return 0;
}
