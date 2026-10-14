// 练习 5-11 的答案：两条规矩都写在门里。
//
// addScore 是外面唯一能改分数的路，所以两条规矩都塞在这儿：
// 负数直接不理（return 出去），超过 100 封顶。
// 这题的重点不是语法，是「门该开在哪儿」——外面碰不到 score，
// 就永远绕不过这两条规矩。

#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string name, int score) {
        this->name = name;
        this->score = score;
    }

    int getScore() {
        return score;
    }

    void addScore(int d) {
        if (d < 0) {
            return;
        }
        score = score + d;
        if (score > 100) {
            score = 100;
        }
    }
};

int main() {
    Student a("小明", 80);

    a.addScore(-50);
    std::cout << a.getScore() << std::endl;

    a.addScore(30);
    std::cout << a.getScore() << std::endl;

    return 0;
}
