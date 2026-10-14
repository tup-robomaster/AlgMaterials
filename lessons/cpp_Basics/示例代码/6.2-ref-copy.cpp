// 6.2 引用改的永远是本人
//
// 两句都写了「a」，可结果不一样：
// 走 b 改的是抄件，走 r 改的是 a 本人。

#include <iostream>

int main() {
    int a = 1;

    int b = a;
    int& r = a;

    b = 100;        // 改抄件
    r = 200;        // 改本人

    std::cout << "a = " << a << "   b = " << b << std::endl;

    return 0;
}
