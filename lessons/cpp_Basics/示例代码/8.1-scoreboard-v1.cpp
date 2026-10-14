// 8.1 例题第一版：三件事都做了，只有平均那一行差一位小数。
//
// 它编译一声不吭（下面这行看不出来吧？），跑起来也好好的。
// 病根在 double avg = sum / n; 那一行——
// 等号右边先算完：两个 int 相除，小数当场砍掉，再把那个整数放进 double。

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

    double avg = sum / n;

    std::cout << "最高：" << mx << std::endl;
    std::cout << "最低：" << mn << std::endl;
    printf("平均：%.1f\n", avg);

    return 0;
}
