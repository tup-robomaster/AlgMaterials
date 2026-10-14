// 练习 1-11 的答案：第一行 6，第二行 7。
//
// 7 / 2 是两个 int 相除，先算出 3，再跟 2.0 乘，得 6.0。
// 7 * 2.0 因为碰上 double 先变成 14.0，再除以 2，得 7.0。
// 谁先算，谁决定中间那一步是整数还是小数。

#include <iostream>

int main() {
    std::cout << 7 / 2 * 2.0 << std::endl;
    std::cout << 7 * 2.0 / 2 << std::endl;
    return 0;
}
