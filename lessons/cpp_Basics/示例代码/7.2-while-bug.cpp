// 练习 7-10 正文里贴的那一份（错）：while 的边界少了一轮。
// 该打「1 到 5 的和：15」，它打 10。

#include <iostream>

int main() {
    int n = 5;
    int sum = 0;
    int i = 1;

    while (i < n) {
        sum = sum + i;
        i = i + 1;
    }

    std::cout << "1 到 5 的和：" << sum << std::endl;

    return 0;
}
