// 1.2 类型 —— 补充：int 装不下的时候
//
// int 能装的最大值是 2147483647。再加 1 会怎样？

#include <iostream>

int main() {
    int a = 2147483647;      // int 能装的最大值
    std::cout << a << std::endl;

    a = a + 1;               // 再加 1
    std::cout << a << std::endl;

    return 0;
}
