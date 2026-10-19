// 1.4 用：1.1 那个大 main 里，中间那几件活搬到这儿来兑现。
//
// stats.h 里写过的每一个承诺，这儿都得有一份对得上的实现：
// 名字、参数类型、返回类型，一个都不能差。

#include "stats.h"

#include <cstdio>
#include <iostream>

std::vector<int> readScores(int n) {
    std::vector<int> scores(n);
    for (int i = 0; i < n; i++) {
        std::cin >> scores[i];
    }
    return scores;
}

double average(std::vector<int>& scores) {
    int sum = 0;
    for (int i = 0; i < int(scores.size()); i++) {
        sum = sum + scores[i];
    }
    return sum / double(scores.size());
}

void printReport(std::vector<int>& scores) {
    int sum = 0;
    int top = scores[0];
    int low = scores[0];
    for (int i = 0; i < int(scores.size()); i++) {
        sum = sum + scores[i];
        if (scores[i] > top) {
            top = scores[i];
        }
        if (scores[i] < low) {
            low = scores[i];
        }
    }

    int excellent = 0;
    int good = 0;
    int fair = 0;
    int pass = 0;
    int fail = 0;
    for (int i = 0; i < int(scores.size()); i++) {
        if (scores[i] >= 90) {
            excellent++;
        } else if (scores[i] >= 80) {
            good++;
        } else if (scores[i] >= 70) {
            fair++;
        } else if (scores[i] >= 60) {
            pass++;
        } else {
            fail++;
        }
    }

    printf("分数    ");
    for (int i = 0; i < int(scores.size()); i++) {
        printf("%4d", scores[i]);
    }
    printf("\n");
    printf("总分    %4d\n", sum);
    printf("平均    %4.1f\n", average(scores));
    printf("最高    %4d\n", top);
    printf("最低    %4d\n", low);
    printf("优      %4d\n", excellent);
    printf("良      %4d\n", good);
    printf("中      %4d\n", fair);
    printf("及格    %4d\n", pass);
    printf("不及格  %4d\n", fail);
}
