// 跟 5.4-protected.cpp 是同一个程序，最后多了一行：main 里碰 score
//
// 这一行编译不过——protected 挡的不是子类，是「外面」。
// 加上 5.4-private-err.cpp（子类碰 private 也不行），
// 三档的界线就齐了：
//
//   public      谁都能碰
//   protected   子类能碰，外面不能
//   private     只有自己家能碰，子类也不行
//
// 故意写得极简，为的是让报错里的行号能和课里贴的源码逐行对上。

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

    b.score = 100;

    return 0;
}
