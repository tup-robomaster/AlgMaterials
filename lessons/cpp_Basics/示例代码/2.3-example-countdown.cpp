// 2.3 例题：读一个数，从它倒着打到 1。
//
// 循环变量这次是往下走的：n = n - 1。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    while (n >= 1) {
        std::cout << n << std::endl;
        n = n - 1;
    }

    std::cout << "发射" << std::endl;

    return 0;
}
