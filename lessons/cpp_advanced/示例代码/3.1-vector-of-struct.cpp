// 3.1：vector 里装什么，是尖括号那一格里说了算的。
// 装 int 行，装 string 行，装你自己定义的类型当然也行。
//
// 这是往后看真实代码最常见的一行——「一个装着若干目标的 vector」就是这个句式。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-vector-of-struct.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <vector>

class Student {
public:
    std::string name;
    int score;

    Student(std::string n, int s) {
        name = n;
        score = s;
    }
};

int main() {
    std::vector<Student> roster;          // 尖括号里是 Student，不是 int

    Student a("小明", 90);
    roster.push_back(a);

    roster.push_back(Student("小红", 85)); // 连名字都不用起，当场构造一个对象

    for (Student& s : roster) {
        s.score = s.score + 5;
    }

    for (Student& s : roster) {
        std::cout << s.name << " " << s.score << std::endl;
    }

    return 0;
}
