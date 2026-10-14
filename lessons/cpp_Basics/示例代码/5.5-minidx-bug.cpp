// 5.5【综合练习二·类版】—— 这一份是错的
//
// 编译干净，一个警告都没有跑出来。
// 喂进去 5 1 9 3 7 4，它打的是 1 9 4 7 3，应该是 4 9 3 7 1。
//
// 最坑的是：换回题目给的那组样例（5 34 1 2 56 4），它又是对的。
// 换一组就露馅。
//
// 输入在 5.5-minidx-bug.in 里。

#include <iostream>

class MyClass {
public:
    void fun(int a[], int n) {
        int minIdx = 1;

        for (int i = 1; i < n; i++) {
            if (a[i] < a[minIdx]) {
                minIdx = i;
            }
        }

        int t = a[minIdx];
        a[minIdx] = a[n - 1];
        a[n - 1] = t;
    }
};

int main() {
    int a[100];
    int n;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    // 交换前
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    MyClass m;
    m.fun(a, n);

    // 交换后
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
