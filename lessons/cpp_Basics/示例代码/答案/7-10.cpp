// 练习 7-10（加打印）：while 的边界少了一轮。
//
// 在循环开头打一句 i，看它每轮是几。
// 打出来停在 4 上，5 从来没进过循环——因为条件是 i < n，
// i 走到 5 的时候就停了。要的是 1 到 5，得写成 i <= n。

#include <iostream>

int main() {
    int n = 5;
    int sum = 0;
    int i = 1;

    while (i <= n) {
        std::cout << "这一轮 i = " << i << std::endl;
        sum = sum + i;
        i = i + 1;
    }

    std::cout << "1 到 5 的和：" << sum << std::endl;

    return 0;
}
