// 练习 6-6 的答案：指针读一次、写一次
//
// *p 是「顺着 p 里的地址找过去」。读它读到 7，写它就是写 a。

#include <iostream>

int main() {
    int a = 7;
    int* p = &a;

    std::cout << *p << std::endl;

    *p = 70;

    std::cout << a << std::endl;

    return 0;
}
