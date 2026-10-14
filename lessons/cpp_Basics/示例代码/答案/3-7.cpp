// 练习 3-7 的答案：出了地盘的那一行，得搬回地盘里。
//
// 原来错在最后那句 std::cout << n —— n 的盒子在大括号关掉的时候就没了。
// 改法：把要用 n 的那句话搬进大括号里（total 不用搬，它本来就住外面）。

#include <iostream>

int main() {
    int total = 0;

    {
        int n = 5;
        total = total + n;
        std::cout << "里面：n = " << n << "，total = " << total << std::endl;
    }

    std::cout << "外面：total = " << total << std::endl;
    return 0;
}
