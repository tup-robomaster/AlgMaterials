// 2.1：第一个 if。58 分没到 60，「及格」那句不走。
//
// 把 58 改成 75 再跑一次，会多打一行「及格」。

#include <iostream>

int main() {
    int score = 58;

    if (score >= 60) {
        std::cout << "及格" << std::endl;
    }

    std::cout << "看完了" << std::endl;

    return 0;
}
