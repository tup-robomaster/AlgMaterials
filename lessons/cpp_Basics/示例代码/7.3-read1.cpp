// 练习 7-11 正文里贴的那一份：读它，先猜它打什么，再编译。
// 这一份没有任何错，就是要你练「不跑也能读出来」。

#include <iostream>

int main() {
    int a[6] = {3, 1, 4, 1, 5, 9};
    int k = 0;

    for (int i = 0; i < 6; i++) {
        if (a[i] > a[k]) {
            k = i;
        }
    }

    std::cout << k << " " << a[k] << std::endl;

    return 0;
}
