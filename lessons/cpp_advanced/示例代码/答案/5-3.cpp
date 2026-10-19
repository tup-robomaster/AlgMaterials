// 练习 5-3 的答案：一块内存存放 n 个数，找出最大值和最小值，再释放。
//
// 最大值和最小值都从第一个数开始（不是从 0 开始）——
// 从 0 开始的话，全是负数的一组数会得到 0，那就错了。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int* data = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> data[i];
    }

    int big = data[0];
    int small = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] > big) {
            big = data[i];
        }
        if (data[i] < small) {
            small = data[i];
        }
    }

    std::cout << "最高 " << big << std::endl;
    std::cout << "最低 " << small << std::endl;

    delete[] data;
    data = nullptr;

    return 0;
}
