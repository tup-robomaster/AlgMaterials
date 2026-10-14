// 练习 6-11 的答案：用指针把数组走一遍
//
// *(a + i) 跟 a[i] 是一回事——a[i] 本来就是「a 的地址往后挪 i 格，
// 再把那格拿出来」的简写。这也是 4.2 说「下标就是从开头数第几个」的由来。

#include <iostream>

int main() {
    int a[4] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        std::cout << *(a + i) << " ";
    }
    std::cout << std::endl;

    return 0;
}
