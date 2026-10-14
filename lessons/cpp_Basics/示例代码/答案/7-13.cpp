// 练习 7-13：打的是「最大的是 30」。
//
// 读法：第一个 cin 读进来的是 n，也就是「接下来有几个数」，
// 它自己不是数据。后面那 n 个才是要挑的数：12 7 30 4 19，最大的是 30。

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
