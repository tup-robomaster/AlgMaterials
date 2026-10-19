// 练习 1-1：把 1.1 那份自己敲一遍。
//
// 答案就是课里那一份，一个字母都没改——这道题要的是手感，不是写出新东西。
// 敲完看一眼屏幕上那张表，和课里贴的那份对一遍。
//
// 输入在 1-1.in 里（十个分数），跑的时候喂进去：
//     g++ -std=c++17 -Wall 1-1.cpp -o /tmp/t && /tmp/t < 1-1.in

#include <cstdio>
#include <iostream>
#include <vector>

int main() {
    // 十个分数，一个一个读进来
    std::vector<int> scores(10);
    for (int i = 0; i < 10; i++) {
        std::cin >> scores[i];
    }

    // 总分、最高、最低：一遍扫完
    int total = 0;
    int top = scores[0];
    int low = scores[0];
    for (int i = 0; i < 10; i++) {
        total = total + scores[i];
        if (scores[i] > top) {
            top = scores[i];
        }
        if (scores[i] < low) {
            low = scores[i];
        }
    }

    // 平均分：又单独扫了一遍
    int totalAgain = 0;
    for (int i = 0; i < 10; i++) {
        totalAgain = totalAgain + scores[i];
    }
    double avg = totalAgain / 10.0;

    // 五档各几个
    int excellent = 0;
    int good = 0;
    int fair = 0;
    int pass = 0;
    int fail = 0;
    for (int i = 0; i < 10; i++) {
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

    // 输出统计表
    printf("分数    ");
    for (int i = 0; i < 10; i++) {
        printf("%4d", scores[i]);
    }
    printf("\n");
    printf("总分    %4d\n", total);
    printf("平均    %4.1f\n", avg);
    printf("最高    %4d\n", top);
    printf("最低    %4d\n", low);
    printf("优      %4d\n", excellent);
    printf("良      %4d\n", good);
    printf("中      %4d\n", fair);
    printf("及格    %4d\n", pass);
    printf("不及格  %4d\n", fail);

    return 0;
}
