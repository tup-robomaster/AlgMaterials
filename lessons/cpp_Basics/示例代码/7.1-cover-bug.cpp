// 练习 7-1 正文里贴的那一份（错）：累加写成了覆盖。
// 该打「总和：100」，它打「总和：40」。

#include <iostream>

int main() {
    int a[4] = {10, 20, 30, 40};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum = a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
