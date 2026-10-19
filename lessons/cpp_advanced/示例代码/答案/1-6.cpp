// 练习 1-6：把 1.1 那份拆成三个函数，写在一个文件里。
//
// 拆完 main 里只剩两句：读、打。
// 平均分没在 main 里算——printReport 自己要打那一行，它自己调 average。
// （1.3 那个目录里的 stats.cpp 就是这么摆的，你手上这三块和它是同一份东西，
//   区别只是这三块和 main 挤在一个文件里。）
//
// 输出和 1.1 一字不差。跑完自己 diff 一下：
//     ./report < 1-6.in > 新.txt
//     ./all-in-one < 1.1-all-in-one.in > 旧.txt   （1.1 那份编出来的）
//     diff 新.txt 旧.txt

#include <cstdio>
#include <iostream>
#include <vector>

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

int main() {
    std::vector<int> scores = readScores(10);

    printReport(scores);
    return 0;
}
