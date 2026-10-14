// 检测题 5 题面里那份「及格人数少了一个」的程序。
//
// 编译干净、跑得正常，最高分那一行还是对的。
// 病根是 countPass 里那个 1——它跟 4.2 讲的「下标从 0 开始数」是同一件事的两面。

#include <iostream>

int countPass(int a[], int n) {
    int c = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] >= 60) {
            c++;
        }
    }
    return c;
}

int main() {
    int n;
    int a[20];

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int mx = a[0];
    for (int i = 0; i < n; i++) {
        if (a[i] > mx) {
            mx = a[i];
        }
    }

    std::cout << "最高：" << mx << std::endl;
    std::cout << "及格：" << countPass(a, n) << std::endl;

    return 0;
}
