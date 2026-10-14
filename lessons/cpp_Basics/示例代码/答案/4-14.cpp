// 练习 4-14 的答案：找最小值，和【第一个】交换。
//
// 跟综合练习二只差一个下标：a[n - 1] 换成 a[0]。
// 找最小和交换那两段，一个字都没改。
//
// 打印那两段也跟正解一样：交换前一行、交换后一行。
// 换的是第 0 个，所以「交换后」那一行的开头会变。
//
// 输入在 4-14.in 里：5 4 9 1 7 3

#include <iostream>

int main() {
    int a[100];
    int n;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int minIdx = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[minIdx]) {
            minIdx = i;
        }
    }

    // 交换前
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    int t = a[minIdx];
    a[minIdx] = a[0];
    a[0] = t;

    // 交换后
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
