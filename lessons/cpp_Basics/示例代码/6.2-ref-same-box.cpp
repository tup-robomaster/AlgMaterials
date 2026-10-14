// 6.2 引用不是新盒子，是同一个盒子的另一个名字
//
// b 是新盒子（copy 了一份值），r 不是盒子（就是 a 本人）。
// 怎么证明？看地址——同一个盒子只有一个地址。

#include <iostream>

int main() {
    int a = 3;

    int b = a;      // 新开一个盒子 b，把 a 里的值抄一份放进去
    int& r = a;     // r 不是新盒子，它是 a 的另一个名字

    std::cout << "b 跟 a 是同一个盒子吗  " << (&b == &a) << std::endl;
    std::cout << "r 跟 a 是同一个盒子吗  " << (&r == &a) << std::endl;

    return 0;
}
