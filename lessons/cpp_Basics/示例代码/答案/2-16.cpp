// 练习 2-16 答案：先读 n，再读 n 个数，把它们加起来。
//
// 循环里套了个 cin——这是第一次「一边转一边从键盘拿数」。
// 每转一圈读一个，读完 n 个，循环也正好转完 n 圈。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int sum = 0;
    for (int i = 1; i <= n; i = i + 1) {
        int x = 0;
        std::cin >> x;
        sum = sum + x;
    }

    std::cout << sum << std::endl;

    return 0;
}
