// 练习 5-19 的答案：同一个盒子，换一件事干。
//
// 只改了两处：比较的方向（< 变 >），还有变量的名字（minIdx 变 maxIdx）。
// 位置那套写法一个字都没动——找最大和找最小是同一件事，
// 只是那个小于号朝哪边。
//
// 函数名还是 fun：题目把名字定死了，换活儿不换名。
//
// 输入在 5-19.in 里：5 34 1 2 56 4

#include <iostream>

class MyClass {
public:
    void fun(int a[], int n) {
        int maxIdx = 0;
        for (int i = 1; i < n; i++) {
            if (a[i] > a[maxIdx]) {
                maxIdx = i;
            }
        }

        int t = a[maxIdx];
        a[maxIdx] = a[n - 1];
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
