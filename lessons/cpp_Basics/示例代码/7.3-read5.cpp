// 练习 7-15 正文里贴的那一份（错）：同一个函数，两次调用的参数不一样。
// 「总和」那一行是对的，「平均」那一行不对。

#include <iostream>

int f1(int a[], int n) {
    int s = 0;

    for (int i = 0; i < n; i++) {
        s = s + a[i];
    }

    return s;
}

int main() {
    int a[5] = {10, 20, 30, 40, 50};

    std::cout << "总和：" << f1(a, 5) << std::endl;
    std::cout << "平均：" << f1(a, 4) / 4 << std::endl;

    return 0;
}
