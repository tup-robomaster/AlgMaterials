// 1.4 用：一个只放「承诺」的头文件——函数叫什么、收什么、还什么，
// 一个字的实现都没有。实现都写在 stats.cpp 里。
//
// main.cpp 把这份清单贴进去，才知道 readScores 该收几个参数。
// 卫士那三行（#ifndef / #define / #endif）是 1.2 讲的，这里照抄。

#ifndef STATS_H
#define STATS_H

#include <vector>

// 读 n 个分数回来
std::vector<int> readScores(int n);

// 平均分
double average(std::vector<int>& scores);

// 输出统计表：分数、总分、平均、最高、最低、五档各几个
void printReport(std::vector<int>& scores);

#endif
