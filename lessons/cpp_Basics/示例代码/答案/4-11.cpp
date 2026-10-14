// 练习 4-11 的答案：交换第 2 个和第 4 个。
//
// 「第 2 个」的编号是 1，「第 4 个」的编号是 3——先在纸上把这一步定下来。
// 打印用 4.4 例题那个写法：先单打 a[0]，再从 1 起每个前面补一个空格。

#include <iostream>

int main() {
    int a[5] = {10, 20, 30, 40, 50};

    int t = a[1];
    a[1] = a[3];
    a[3] = t;

    std::cout << a[0];
    for (int i = 1; i < 5; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
