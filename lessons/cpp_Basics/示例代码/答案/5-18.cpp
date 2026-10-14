// 练习 5-18 的答案：再给盒子加一个动作，交回总和。
//
// sum 跟 fun 是兄弟，都住在盒子里——可数据不在盒子里，
// 在 main 那儿。所以它也得收 a[] 和 n 才看得见那串数。
// （5.5 版本二那种装法里，它才能一个参数都不收。）
//
// 顺序是：交换前、交换后、总和。换位置不改总和，
// 所以那一行数字放在哪儿都一样，这里按题目的顺序排在最后。
//
// 输入在 5-18.in 里：5 34 1 2 56 4

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

    int sum(int a[], int n) {
        int total = 0;
        for (int i = 0; i < n; i++) {
            total = total + a[i];
        }
        return total;
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

    std::cout << m.sum(a, n) << std::endl;

    return 0;
}
