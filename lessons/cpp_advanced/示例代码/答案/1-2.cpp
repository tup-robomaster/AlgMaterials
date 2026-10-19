// 练习 1-2：在 1.1 那份上加一行「及格率」。
//
// 就多了一行 printf。关键在那句话：一个数都不用重新数——上面那五个计数器
// 已经把十个人分完了，及格的就是前四档加起来。
// 这题练的不是算术，是「在别人的文件里找到已经有的东西」。
//
// 90.0% 那个百分号：printf 里要写两个 %% 才打得出一个 %，
// 单写一个会被当成格式符的开头。除数是 10——在括号里乘了 100.0 再除，
// 已经把它变成小数运算了，不会走整数除法。
//
// （1.1 里那个 totalAgain 我没动。它碍眼，但这题只让你加一行；
//   要收拾它，等练习 1-6 拆函数的时候一起。）

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
    printf("及格率  %4.1f%%\n", (excellent + good + fair + pass) * 100.0 / 10);

    return 0;
}
