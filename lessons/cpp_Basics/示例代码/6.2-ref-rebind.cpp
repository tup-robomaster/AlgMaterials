// 6.2 引用绑上了就换不了
//
// r = b; 这一句，看着像「让 r 改去指 b」，其实不是——
// 它是「把 b 的值写给 r」，而 r 就是 a，所以 a 变成了 2。
//
// r 从头到尾都跟着 a。想换指谁，那是指针干的活（6.3）。

#include <iostream>

int main() {
    int a = 1;
    int b = 2;

    int& r = a;

    r = b;          // 把 b 的值写给 r（也就是写给 a），不是让 r 改去指 b

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "r 还跟着 a 吗  " << (&r == &a) << std::endl;

    return 0;
}
