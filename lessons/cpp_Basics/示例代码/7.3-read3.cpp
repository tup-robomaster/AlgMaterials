// 练习 7-13 正文里贴的那一份：先读出来它读几个数，再说它打什么。
// 要读输入，输入在 7.3-read3.in 里。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int a[20];
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int mx = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > mx) {
            mx = a[i];
        }
    }

    std::cout << "最大的是 " << mx << std::endl;

    return 0;
}
