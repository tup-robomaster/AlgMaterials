// 8.1 例题正解：跟上一版只差一个字符位置。
//
// sum * 1.0 / n —— 先乘出一个 double，除法就按小数除了。
// 1.3 讲的「只要有一个是小数，结果就是小数」，在这儿刚好用上。
//
// 顺序不能换：写成 1.0 * sum / n 一样对，写成 sum / n * 1.0 就白改了——
// 除号先算，小数还是在那时候被砍掉的。

#include <cstdio>
#include <iostream>

int main() {
    int n;
    int a[20];

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int mx = a[0];
    int mn = a[0];
    int sum = 0;

    for (int i = 0; i < n; i++) {
        if (a[i] > mx) {
            mx = a[i];
        }
        if (a[i] < mn) {
            mn = a[i];
        }
        sum = sum + a[i];
    }

    double avg = sum * 1.0 / n;

    std::cout << "最高：" << mx << std::endl;
    std::cout << "最低：" << mn << std::endl;
    printf("平均：%.1f\n", avg);

    return 0;
}
