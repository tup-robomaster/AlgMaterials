// 练习 7-1（找错）：累加写成了覆盖。
//
// 病根那一行是 sum = a[i];——每一轮都把上一轮的结果冲掉了，
// 循环跑完，sum 里只剩下最后一个数 40。
// 改成 sum = sum + a[i]; 就打 100。

#include <iostream>

int main() {
    int a[4] = {10, 20, 30, 40};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
