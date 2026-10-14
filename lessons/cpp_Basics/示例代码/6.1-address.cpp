// 6.1 变量住在内存里 —— 每个盒子都有一块地方，那块地方有编号
//
// 跑一下：g++ -std=c++17 -Wall 示例代码/6.1-address.cpp -o /tmp/t && /tmp/t
//
// 打出来的那个地址，你每次跑都不一样，换台机器更不一样。
// 这是正常的，课里专门讲了这件事——**别把某个具体地址记下来**。

#include <iostream>

int main() {
    int a = 3;

    std::cout << "a 里装的   " << a << std::endl;
    std::cout << "a 的地址   " << &a << std::endl;
    std::cout << "a 占几格   " << sizeof(a) << std::endl;

    return 0;
}
