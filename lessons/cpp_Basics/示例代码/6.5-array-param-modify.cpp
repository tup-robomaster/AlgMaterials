// 6.5 数组能被改，是因为递进去的是门牌号
//
// 跟 6.4-pointer.cpp 是同一个道理：函数改的是那一排本身。
// 这一份把「为什么」写清楚——参数表上的 int a[]，就是 int* a。

#include <iostream>

void doubleAll(int a[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = a[i] * 2;
    }
}

int main() {
    int a[3] = {1, 2, 3};

    doubleAll(a, 3);

    for (int i = 0; i < 3; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
