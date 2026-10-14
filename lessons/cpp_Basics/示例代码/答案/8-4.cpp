// 练习 8-4 答案：把一排数倒过来。
//
// reverse 里那个 i < n / 2 是这一题的关键：走到一半就该停。
// 走满 n 圈会把刚换好的又换回去，看起来「没换」——
// 前半截全对，后半截全错。
//
// 两个下标一个从前往后（i），一个从后往前（n - 1 - i），
// 碰头的地方就是 n / 2。n 是奇数的时候中间那个自己跟自己换，不用管。
//
// swap2 参数表上写的是 int&，不是 int。写成 int 就换成抄件了，
// 数组里一点动静都没有（3.4 和 6.2 讲过这件事）。

#include <iostream>

void swap2(int& x, int& y) {
    int t = x;
    x = y;
    y = t;
}

void reverse(int a[], int n) {
    for (int i = 0; i < n / 2; i++) {
        swap2(a[i], a[n - 1 - i]);
    }
}

void printAll(int a[], int n) {
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;
}

int main() {
    int n;
    int a[20];

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    printAll(a, n);
    reverse(a, n);
    printAll(a, n);

    return 0;
}
