// 4.1 数组 —— 补充：再往后写一格，程序就死了
//
// 和 4.1-write-oob.cpp 是同一件事，只多写了 score[6] 那一行。
// 那一格压到了编译器放在栈上的哨兵，程序收尾时自己发现了，
// 打一行 stack smashing 就中止，退出码 134。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    score[5] = 88;
    score[6] = 99;                    // 只多了这一行

    std::cout << "第一个 " << score[0] << std::endl;

    return 0;
}
