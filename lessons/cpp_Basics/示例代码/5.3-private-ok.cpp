// 5.3 构造函数 —— private 值钱的地方
//
// 成绩藏起来以后，外面想加分只能走 addScore 这道门。
// 而门是盒子里自己管的：加到超过 100 就封顶。
//
// 要是 score 是公开的，外面一句 a.score = 105; 谁也拦不住。

#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string n, int s) {
        name = n;
        score = s;
    }

    int getScore() {
        return score;
    }

    void addScore(int d) {
        score = score + d;
        if (score > 100) {
            score = 100;
        }
    }
};

int main() {
    Student a("小明", 95);

    a.addScore(10);

    std::cout << a.getScore() << std::endl;

    return 0;
}
