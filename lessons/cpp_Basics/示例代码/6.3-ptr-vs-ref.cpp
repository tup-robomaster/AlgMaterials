// 6.3 指针和引用，一句话说清区别
//
//   引用：绑上就换不了，用起来跟普通变量一样（不用写 *）
//   指针：随时能换指谁，但每次用都得自己写个 *
//
// 两份代码干的是同一件事（把 b 的值给 a），看一眼区别在哪。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;

    int* p = &a;
    p = &b;             // 指针：换个地址装进去就行了
    *p = 20;            // 顺着新地址写——写的是 b

    int& r = a;
    r = 30;             // 引用：直接写，它就是 a

    std::cout << "a = " << a << "   b = " << b << std::endl;
    std::cout << "p 指着 b 吗  " << (p == &b) << std::endl;
    std::cout << "r 跟着 a 吗  " << (&r == &a) << std::endl;

    return 0;
}
