// 练习 7-7 正文里贴的那一份（错，但作者自己加了一句打印）。
// 该打「总和：24」，它打「总和：21」。
// 打印留在里面了，正好拿它当线索。

#include <iostream>

int main() {
    int a[5] = {4, 7, 1, 9, 3};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        std::cout << "第 " << i << " 轮：加上 " << a[i] << std::endl;
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
