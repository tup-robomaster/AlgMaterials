// 5.2 class —— 加上 public: 就过了
//
// 跟上一份的差别只有两处：class 换掉 struct，加了 public: 那一行。
// 后面住的还是同样的东西。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
    }
};

int main() {
    Student a;
    a.name = "小明";
    a.id = 101;
    a.score = 90;

    a.print();

    return 0;
}
