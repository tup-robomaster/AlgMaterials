// 7.2 打印调试 —— 正文里那份「总和不对」的程序。
//
// 四个数加起来该是 14，它打 4。
// 这一份光用眼睛看很难看出来，加一句打印就现形了（见 7.2-sum-print.cpp）。

#include <iostream>

int main() {
    int a[4] = {3, 5, 2, 4};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum = 0;
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
