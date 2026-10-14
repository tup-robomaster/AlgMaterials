// 1.3 运算 —— 补充：除以 0 会怎样
//
// 这个程序会崩。故意留着，让你见一次真实的样子。

#include <iostream>

int main() {
    int a = 7;
    int b = 0;

    std::cout << "算之前" << std::endl;
    std::cout << a / b << std::endl;      // 这一行会出事
    std::cout << "算之后" << std::endl;   // 永远不会跑到这里

    return 0;
}
