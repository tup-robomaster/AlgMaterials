// 练习 7-15（读别人的代码，里面有个错）：两次调的 n 不一样。
//
// 第一行传的是 5，第二行传的是 4，还拿 4 去除——
// 函数没错，错在调用它的那一行。两处都改成 5 就对了。

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
    std::cout << "平均：" << f1(a, 5) / 5 << std::endl;

    return 0;
}
