// 6.4 加了 const 的引用，就改不动了
//
// const 的意思是「只许读，不许改」。参数上写 const int&，
// 是给调用的人一个保证：**这个函数只是看看，不会动我的东西。**
//
// 函数里真去改它，编译器当场拦下。

#include <iostream>

void show(const int& x) {
    x = 5;                        // 错：x 是只读的
    std::cout << x << std::endl;
}

int main() {
    int a = 3;

    show(a);

    return 0;
}
