// 练习 6-12 的答案：收指针的求和函数
//
// 参数写成 int* a 和 int a[] 是一回事（6.5 那份警告证明了这一点）。
// 这里故意用指针的写法，让你看清递进去的到底是什么。

#include <iostream>

int sum(int* a, int n) {
    int total = 0;

    for (int i = 0; i < n; i++) {
        total = total + a[i];
    }

    return total;
}

int main() {
    int a[4] = {10, 20, 30, 40};

    std::cout << sum(a, 4) << std::endl;

    return 0;
}
