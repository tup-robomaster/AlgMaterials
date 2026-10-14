// 练习 7-8（加打印）：mx 从头到尾没变过。
//
// 在循环开头打一句 mx，四轮打出来全是 12——说明 if 里头
// 只改了 idx，忘了改 mx。把 mx = a[i]; 补上。

#include <iostream>

int main() {
    int a[5] = {12, 45, 7, 33, 28};
    int mx = a[0];
    int idx = 0;

    for (int i = 1; i < 5; i++) {
        std::cout << "第 " << i << " 轮：a[" << i << "] = " << a[i]
                  << "，mx = " << mx << std::endl;
        if (a[i] > mx) {
            mx = a[i];
            idx = i;
        }
    }

    std::cout << "最大值：" << mx << std::endl;
    std::cout << "它在第 " << idx << " 个" << std::endl;

    return 0;
}
