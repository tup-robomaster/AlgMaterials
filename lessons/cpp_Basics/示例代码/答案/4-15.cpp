// 练习 4-15 的答案：找最大值，和最后一个交换。
//
// 跟综合练习二只差一个比较符号：< 换成 >，minIdx 换成 maxIdx。
// 「和最后一个交换」那三行原样不动。
//
// 打印那两段也跟正解一样：交换前一行、交换后一行。
//
// 输入在 4-15.in 里：5 4 9 1 7 3

#include <iostream>

int main() {
    int a[100];
    int n;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int maxIdx = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] > a[maxIdx]) {
            maxIdx = i;
        }
    }

    // 交换前
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    int t = a[maxIdx];
    a[maxIdx] = a[n - 1];
    a[n - 1] = t;

    // 交换后
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
