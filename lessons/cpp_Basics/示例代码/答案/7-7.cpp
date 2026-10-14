// 练习 7-7（从打印里读病根）：循环少跑了一轮。
//
// 打印里的最后一轮是「第 3 轮」，说明循环只跑了四圈，
// 下标 4 那个格子（里面是 3）从头到尾没被加进去。
// 上限从 4 改成 5 就对了，打印留着没动。

#include <iostream>

int main() {
    int a[5] = {4, 7, 1, 9, 3};
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        std::cout << "第 " << i << " 轮：加上 " << a[i] << std::endl;
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
