// 练习 7-12：这个函数把所有偶数加起来。
//
// 读法：先看返回值 s 是怎么攒起来的——只有 a[i] % 2 == 0 的时候才加。
// 4 + 2 + 6 = 12，7 和 9 是奇数，跳过去了。

#include <iostream>

int g(int a[], int n) {
    int s = 0;

    for (int i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            s = s + a[i];
        }
    }

    return s;
}

int main() {
    int a[5] = {4, 7, 2, 9, 6};

    std::cout << g(a, 5) << std::endl;

    return 0;
}
