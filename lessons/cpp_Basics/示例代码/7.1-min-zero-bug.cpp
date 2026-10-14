// 练习 7-2 正文里贴的那一份（错）：最小值从 0 开始找。
// 该打「最小值：3」，它打「最小值：0」。

#include <iostream>

int main() {
    int a[5] = {7, 3, 9, 4, 6};
    int mn = 0;

    for (int i = 0; i < 5; i++) {
        if (a[i] < mn) {
            mn = a[i];
        }
    }

    std::cout << "最小值：" << mn << std::endl;

    return 0;
}
