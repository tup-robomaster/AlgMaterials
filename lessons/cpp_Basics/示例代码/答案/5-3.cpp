// 练习 5-3 的答案：找成绩最高的那个。
//
// 跟 4.3 的擂台一模一样，只是「分数」换成了「整个学生」：
// 擂台上站的不再是分数，是一个学生，比的是他的 score。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    int score;
};

int main() {
    Student s[3];

    s[0].name = "小明";
    s[0].id = 101;
    s[0].score = 90;

    s[1].name = "小红";
    s[1].id = 102;
    s[1].score = 85;

    s[2].name = "小刚";
    s[2].id = 103;
    s[2].score = 95;

    int best = 0;
    for (int i = 1; i < 3; i++) {
        if (s[i].score > s[best].score) {
            best = i;
        }
    }

    std::cout << s[best].name << std::endl;

    return 0;
}
