// 练习 4-3 的答案：三个 double，改中间那个。
//
// 最后一行打出来是 1.8 而不是 1.80——cout 默认不管末尾的 0（1.4 讲过）。
// 要 1.80 就得请 1.6 的 printf 出马。

#include <iostream>

int main() {
    double h[3] = {1.75, 1.62, 1.80};

    h[1] = 1.68;

    std::cout << h[0] << std::endl;
    std::cout << h[1] << std::endl;
    std::cout << h[2] << std::endl;

    return 0;
}
