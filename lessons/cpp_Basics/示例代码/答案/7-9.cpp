// 练习 7-9（打完要删掉）：数是对的，多出来的是调试打印。
//
// 三行 DEBUG 是调的时候加的，毛病找完就该删。
// 删掉之后，输出里只剩该打的那一行。

#include <iostream>

int main() {
    int a[3] = {2, 4, 6};
    int sum = 0;

    for (int i = 0; i < 3; i++) {
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
