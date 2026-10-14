// 4.1：不用数组，五个成绩就得开五个盒子。
//
// 这不是编出来吓人的例子——第 3 部分那份作业就是这么写的：
// 一件事要重复几遍，就抄几遍。
//
// 抄几遍能忍，前提是**遍数你事先知道**。真要装一堆数的时候，
// 数的个数是读进来的，抄都不知道该抄几份。

#include <iostream>

int main() {
    int score1 = 90;
    int score2 = 85;
    int score3 = 77;
    int score4 = 60;
    int score5 = 95;

    std::cout << score1 << std::endl;
    std::cout << score2 << std::endl;
    std::cout << score3 << std::endl;
    std::cout << score4 << std::endl;
    std::cout << score5 << std::endl;

    return 0;
}
