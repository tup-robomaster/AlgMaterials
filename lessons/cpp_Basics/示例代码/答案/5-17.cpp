// 练习 5-17 的答案：照题目写的那一份。
//
// 名字照抄题目：类叫 MyClass，那个动作叫 fun。
// fun 只干一件事——找最小值、和末尾交换；读输入和打输出都是 main 的活。
//
// 数组传进函数里，改的是外面那个数组本身（5.5 里讲过这条），
// 所以 fun 换完了，main 打出来的就是换过的那一串。
//
// 输入在 5-17.in 里，就是题目那组样例：5 34 1 2 56 4

#include <iostream>

class MyClass {
public:
    void fun(int a[], int n) {
        int minIdx = 0;
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
