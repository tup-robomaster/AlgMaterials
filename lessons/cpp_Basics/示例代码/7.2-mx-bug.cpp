// 练习 7-8 正文里贴的那一份（错）：找最大值时忘了更新 mx。
// 该打「最大值：45」和「它在第 1 个」，它打 12 和第 4 个。

#include <iostream>

int main() {
    int a[5] = {12, 45, 7, 33, 28};
    int mx = a[0];
    int idx = 0;

    for (int i = 1; i < 5; i++) {
        if (a[i] > mx) {
            idx = i;
        }
    }

    std::cout << "最大值：" << mx << std::endl;
    std::cout << "它在第 " << idx << " 个" << std::endl;

    return 0;
}
