// 练习 6-7 的答案：隔着指针读 b
//
// a = *p * 10; 这句里没出现 b 这个名字，可 *p 就是 b。
// 所以 a 变成 20，b 自己没动。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;

    int* p = &b;

    a = *p * 10;

    std::cout << a << " " << b << std::endl;

    return 0;
}
