// 练习 1-9 的答案：3725 秒 = 1 小时 2 分 5 秒。
//
// 小时数用 /：3725 / 3600 = 1（整数除法把零头砍掉）。
// 零头用 %：3725 % 3600 = 125 秒。
// 再拿这 125 去变分钟：125 / 60 = 2，125 % 60 = 5。

#include <iostream>

int main() {
    int total = 3725;

    int hours = total / 3600;         // 1
    int rest = total % 3600;          // 125
    int minutes = rest / 60;          // 2
    int seconds = rest % 60;          // 5

    std::cout << hours << std::endl;
    std::cout << minutes << std::endl;
    std::cout << seconds << std::endl;
    return 0;
}
