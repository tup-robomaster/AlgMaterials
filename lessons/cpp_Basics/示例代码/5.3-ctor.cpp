// 5.3 构造函数 —— 让盒子出生的时候就装好
//
// Student a("小明", 101, 90); 这一行里，那个构造函数会自动跑一遍，
// 把三样东西填上。从那以后就不会有「忘了赋值」这件事。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;

    Student(std::string n, int i, int s) {
        name = n;
        id = i;
        score = s;
    }

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
    }
};

int main() {
    Student a("小明", 101, 90);

    a.print();

    return 0;
}
