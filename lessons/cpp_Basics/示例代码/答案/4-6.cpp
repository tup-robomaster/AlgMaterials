// 练习 4-6 的答案：把 i <= 5 改成 i < 5。
//
// 五个盒子，合法编号只到 4。写成 <= 5 会让 i 走到 5，
// 去读一个不属于这个数组的位置——编译器不拦，打出来的数是随机的。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    for (int i = 0; i < 5; i++) {
        std::cout << score[i] << std::endl;
    }

    return 0;
}
