// 练习 5-6 的答案：单独调用 isPass()。
//
// 这一题要看的不是结果，是那个调用：s.isPass() —— 交回来一个 bool，
// 跟调用一个普通函数一模一样，只是前面多了个 s.。
// 所以它可以直接放进 if 的括号里。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int score;

    bool isPass() {
        return score >= 60;
    }
};

int main() {
    Student s;
    s.name = "小红";
    s.score = 55;

    if (s.isPass()) {
        std::cout << "及格" << std::endl;
    } else {
        std::cout << "不及格" << std::endl;
    }

    return 0;
}
