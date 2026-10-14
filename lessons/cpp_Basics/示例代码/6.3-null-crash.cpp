// 6.3 用空指针会当场崩 —— 这份故意让它崩
//
// 门牌号是空的，还硬要顺着它找过去，操作系统直接把你拦下来。
// 报错要么是 Segmentation fault，要么是 段错误。
// 崩在这儿其实是好事：比它悄悄读一个乱七八糟的值强得多。

#include <iostream>

int main() {
    int* p = nullptr;

    std::cout << "还没出事" << std::endl;

    std::cout << *p << std::endl;    // 顺着空门牌号找过去

    std::cout << "这一行打不出来了" << std::endl;

    return 0;
}
