// 练习 7-9 正文里贴的那一份：数是对的，可调试打印忘了删。
// 该只打一行「总和：12」，它多嘴了三行 DEBUG。

#include <iostream>

int main() {
    int a[3] = {2, 4, 6};
    int sum = 0;

    for (int i = 0; i < 3; i++) {
        std::cout << "DEBUG i=" << i << std::endl;
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
