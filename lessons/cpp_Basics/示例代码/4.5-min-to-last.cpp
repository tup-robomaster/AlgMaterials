// 4.5【综合练习二】正解：找最小值，和最后一个交换，交换前后各打一行。
//
// 输入的第一个数是**个数**，不是数据。所以第一件事是把它读走，
// 读走之后它就跟这一串数没关系了。
//
// 数组的大小必须是写死的（4.6 讲怎么甩掉这件事），
// 所以先开一个够大的——这是这道题里唯一一处「将就」。
//
// 找最小值那一段用的是 4.3 记住下标的写法：交换要知道位置。
//
//「交换前后各打一行」是照题目原话抄的（题目原话：最终分别打印输出
// 交换前后的数组）。所以打印那一段写了两遍，中间夹着交换那三行。
//
// 输入在 4.5-min-to-last.in 里：5 4 9 1 7 3

#include <iostream>

int main() {
    int a[100];
    int n;

    std::cin >> n;                      // 第一个数是长度

    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int minIdx = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[minIdx]) {
            minIdx = i;
        }
    }

    // 交换前
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    int t = a[minIdx];
    a[minIdx] = a[n - 1];
    a[n - 1] = t;

    // 交换后
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
