// 练习 6-10 的答案：三种传递各来一次，看 a 怎么长
//
// 值传递那次白干（a 还是 1），后两次都改到了本人。

#include <iostream>

void plusOneV(int x)    { x = x + 1; }
void plusOneP(int* p)   { *p = *p + 1; }
void plusOneR(int& x)   { x = x + 1; }

int main() {
    int a = 1;

    plusOneV(a);
    std::cout << a << std::endl;

    plusOneP(&a);
    std::cout << a << std::endl;

    plusOneR(a);
    std::cout << a << std::endl;

    return 0;
}
