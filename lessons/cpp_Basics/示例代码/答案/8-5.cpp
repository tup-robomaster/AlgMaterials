// 练习 8-5 答案：病根就是那个 1。
//
// for (int i = 1; i < n; i++) 把 a[0] 整个跳过去了——
// 第一个格子里的 65 从头到尾没人看过一眼。
//
// 4.2 说下标从 0 开始数，这一题就是那句话的反面：
// 从 1 开始数，等于当第一个数不存在。
//
// 最高分那一行没错，它是在 main 里算的，跟 countPass 一点关系都没有。
// 一个错地方会让后面好几行跟着不对，也有像这样只坏一行的——
// 7.1 那句「第一处不一样的地方，不等于病根」说的是前一种，
// 这一题是后一种：不对的那一行，就是病根露头的地方。

#include <iostream>

int countPass(int a[], int n) {
    int c = 0;
    for (int i = 0; i < n; i++) {
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
