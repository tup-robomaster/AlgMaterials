// 6.3 指针能改去指别人 —— 这是它和引用最大的区别
//
// 同一个 p，先装 a 的地址，后来换成 b 的地址。
// 引用做不到这件事（6.2 那份 6.2-ref-rebind.cpp 里试过了）。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;

    int* p = &a;
    std::cout << "先指着 a：*p = " << *p << std::endl;

    p = &b;                                  // 换个门牌号装进去
    std::cout << "改指 b：  *p = " << *p << std::endl;

    *p = 20;                                 // 这句改的是 b

    std::cout << "a = " << a << "   b = " << b << std::endl;

    return 0;
}
