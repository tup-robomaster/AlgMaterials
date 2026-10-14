// 5.5【综合练习二·类版】—— 照题目写的那一份
//
// 题目里点了两个名字：类叫 MyClass，那个成员函数叫 fun。
// 这两个名字照抄，别自己改——点名要的就是它们。
//
// fun 只干题目让它干的那一件事：找最小值、和末尾交换。
// 读输入、打输出都是 main 的事，题目里「主函数中录入……」
// 和「最终分别打印输出交换前后的数组」说的就是这个。
//
// 输入在 5.5-myclass.in 里，就是题目给的那组样例：
// 5 34 1 2 56 4

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
