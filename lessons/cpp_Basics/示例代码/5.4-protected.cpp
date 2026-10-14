// protected：子类碰得到，外面碰不到
//
// score 设成 protected，GradStudent 的成员函数里可以直接写 score。
// 但 main 里写 b.score 就编译不过——那是「外面」。
//
// 跟 5.4-private-err.cpp 是同一个程序，只差一个关键字。

#include <iostream>

class Student {
protected:
    int score;

public:
    Student(int s) {
        score = s;
    }
};

class GradStudent : public Student {
public:
    GradStudent(int s) : Student(s) {}

    void addBonus() {
        score = score + 5;
    }

    int getScore() {
        return score;
    }
};

int main() {
    GradStudent b(85);

    b.addBonus();

    std::cout << b.getScore() << std::endl;

    return 0;
}
