// 4.1：同样五个成绩，换成数组。
//
// int score[5]; 这一句开出**五个**盒子。它们共用一个名字 score，
// 靠方括号里的编号区分：score[0] 是第一个，score[4] 是第五个。
//
// 方括号里的 5 是**个数**，不是最大的编号。编号是 0 到 4。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    std::cout << score[0] << std::endl;     // 第一个
    std::cout << score[2] << std::endl;     // 第三个

    score[2] = 88;                          // 把第三个改成 88

    std::cout << score[2] << std::endl;     // 再打一次，变了

    return 0;
}
