// 练习 5-1 的答案：申请一块内存，存入 1 到 n，求和，再释放。
//
// 三件事按顺序来：new int[n] 申请内存、循环里当普通数组用、delete[] 释放。
// 释放的是整块内存，所以方括号那一对不能丢——delete data 和 delete[] data
// 不是一回事（释放整块内存要用带方括号的那个）。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int* data = new int[n];
    for (int i = 0; i < n; i++) {
        data[i] = i + 1;
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + data[i];
    }

    std::cout << "1 加到 " << n << " = " << sum << std::endl;

    delete[] data;
    data = nullptr;

    return 0;
}
