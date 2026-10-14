// 4.3：擂台从 0 摆起的后果。
//
// 这份是**错的**。编译干净、跑起来正常，打出来的却是 0——
// 而数组里一个 0 都没有。
//
// 为什么错：0 一上台就再也没下来过。数组里全是负数，
// 没有谁比 0 大，所以那个 if 一次都没进去，台上站到最后的
// 还是那个 0。程序老老实实回答了「谁是最大的」，
// 只是它手里的候选人名单里混进了一个不该在的名字。

#include <iostream>

int main() {
    int score[5] = {-90, -85, -77, -60, -95};

    int maxScore = 0;

    for (int i = 0; i < 5; i++) {
        if (score[i] > maxScore) {
            maxScore = score[i];
        }
    }

    std::cout << maxScore << std::endl;

    return 0;
}
