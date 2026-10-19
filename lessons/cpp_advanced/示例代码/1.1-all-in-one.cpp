// 1.1：一个「什么都塞在 main 里」的程序——这一部分的反面教材。
//
// 读 10 个分数，打一张统计表：分数、总分、平均、最高、最低，外加五档各几个。
// 结果是对的，能编能跑。学生第一次交上来的作业基本就长这样：
// 每一句单看都没错，问题不在对错，在这份文件往后没法长。
//
// 1.2 到 1.5 就是把这个程序拆开。拆完输入一样、输出也一样
// （1.3 会把两份的输出摆在一起比），变的只是它躺在几个文件里。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/1.1-all-in-one.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/1.1-all-in-one.in

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
