// 练习 1-7：分两步编——先 -c 出 .o，再链接。
//
// 三个问题，答案在这儿：
//
// 一、两条命令（文件名随你，这儿就叫 1-7）：
//     g++ -Wall -c 1-7.cpp -o 1-7.o
//     g++ 1-7.o -o report
//   第一条只编不链接（-c），产出 1-7.o；
//   第二条把这个 .o 交给链接器，产出可执行文件。
//
// 二、大小（这台机器上量的，你那几个数会不一样，看个大概就行）：
//    .cpp 约 3 KB，.o 约 22 KB，可执行文件约 23 KB。
//    .o 比 .cpp 大得多——它里面不只是你写的那些代码。到底装了什么，
//    1.4 那个例题用 nm 看过（U 和 T 那两张单子）。
//    从大小倒推不出程序在干什么，第 0 部分说过。
//
// 三、改 main 里打印的那句话——两步都得重走。
//   .o 是「这个文件的编译结果」，源文件改了，它就旧了；
//   链接那一步也得重来，不然装进程序里的还是旧的那个 .o。
//   （程序要是拆成了两个 .cpp，改一个就只重编那一个，另一个不动——
//    这就是 1.4 正文里那组时间戳的样子，也是分开编省下来的时间。）
//
// 程序跟练习 1-6 那份一字不差。

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
