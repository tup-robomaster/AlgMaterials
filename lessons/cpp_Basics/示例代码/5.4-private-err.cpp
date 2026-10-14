// 跟 5.4-protected.cpp 是同一个程序，只差一个关键字：private
//
// 子类里那句 score = score + 5; 编译不过——
// private 是「连子类也不给碰」，protected 才是「子类可以」。
//
// 故意写得极简，为的是让报错里的行号能和课里贴的源码逐行对上。

#include <iostream>

class Student {
private:
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
