// 练习 8-2 答案：最大的那个、它在第几个、比平均高的有几个。
//
// 打擂台管两件事：台上记一个值（mx），另外记一个编号（mxIdx）。
//「比分数，记编号」是 4.3 的原话——这一步写反了就白打。
//
// 平均值那两行是这一题最容易翻车的地方：
// avg 必须是 double，算的时候得先乘出一个小数（sum * 1.0 / n），
// sum / n 两个 int 相除，小数当场就没了。
//
// 位置打的是下标，从 0 数起——4.2 讲的「从开头数第几个」。

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
    int mxIdx = 0;
    int sum = 0;

    for (int i = 0; i < n; i++) {
        if (a[i] > mx) {
            mx = a[i];
            mxIdx = i;
        }
        sum = sum + a[i];
    }

    double avg = sum * 1.0 / n;

    int above = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > avg) {
            above++;
        }
    }

    std::cout << "最大：" << mx << std::endl;
    std::cout << "位置：" << mxIdx << std::endl;
    std::cout << "比平均高：" << above << std::endl;

    return 0;
}
