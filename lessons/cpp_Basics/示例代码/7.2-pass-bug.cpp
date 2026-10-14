// 练习 7-6 正文里贴的那一份（错）：及格人数少数了两个人。
// 该打「及格人数：3」，它打「及格人数：1」。

#include <iostream>

int main() {
    int scores[5] = {60, 55, 72, 60, 48};
    int pass = 0;

    for (int i = 0; i < 5; i++) {
        if (scores[i] > 60) {
            pass = pass + 1;
        }
    }

    std::cout << "及格人数：" << pass << std::endl;

    return 0;
}
