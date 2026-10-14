// 练习 3-5 的答案：sumTo——交回 1 加到 n 的结果。
//
// 和 3.2 的 doubleIt 是同一类：算完不算完，得把结果交回去。
// 注意 sum 是函数自己的盒子，main 里没有它，也不需要知道有它。

#include <iostream>

int sumTo(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i = i + 1) {
        sum = sum + i;
    }
    return sum;
}

int main() {
    std::cout << sumTo(10) << std::endl;
    std::cout << sumTo(100) << std::endl;

    return 0;
}
