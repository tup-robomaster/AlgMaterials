// 7.3 读别人的代码 —— 正文里现场读的那一份。
//
// 名字起得没头没脑，一句注释都没有，这是故意的：
// 别人的代码就长这样。它照样读得懂，靠的是从 main 顺着数据找过去。

#include <iostream>

int f1(int a[], int n) {
    int m = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] < m) {
            m = a[i];
        }
    }

    return m;
}

int f2(int a[], int n, int t) {
    int c = 0;

    for (int i = 0; i < n; i++) {
        if (a[i] == t) {
            c = c + 1;
        }
    }

    return c;
}

int main() {
    int s[8] = {72, 58, 91, 58, 66, 43, 80, 43};
    int n = 8;

    int x = f1(s, n);
    int y = f2(s, n, x);

    std::cout << x << std::endl;
    std::cout << y << std::endl;

    return 0;
}
