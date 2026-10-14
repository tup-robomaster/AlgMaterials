// 2.1：加上 else。两条路，必走一条。
//
// 把 58 改成 75，第一行会变成「及格」。

#include <iostream>

int main() {
    int score = 58;

    if (score >= 60) {
        std::cout << "及格" << std::endl;
    } else {
        std::cout << "不及格" << std::endl;
    }

    return 0;
}
