// 练习 1-10 的答案：507 的百位、十位、个位是 5、0、7。
//
// 先 % 1000 砍掉千位以上，再 / 100 取百位。
// 十位用 / 10 % 10，个位用 % 10。
// 中间那位是 0，别把 / 和 % 的顺序写反——写反了 0 就丢了。

#include <iostream>

int main() {
    int n = 507;

    int hundreds = n / 100 % 10;      // 5
    int tens = n / 10 % 10;           // 0
    int ones = n % 10;                // 7

    std::cout << hundreds << std::endl;
    std::cout << tens << std::endl;
    std::cout << ones << std::endl;
    return 0;
}
