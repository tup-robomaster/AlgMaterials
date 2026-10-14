// 练习 2-13 答案：for 版，打 1 到 n。
//
// 2.3 的练习 2-9 用 while 写过一遍，这里换成 for。
// 三件事从「散在三处」变成「挤在一行里」，干的事一模一样。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 1; i <= n; i = i + 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
