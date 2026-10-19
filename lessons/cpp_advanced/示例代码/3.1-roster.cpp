// 3.1：例题——一张成绩单。
//
// 把这一课的四样东西凑齐用一遍：大括号装好的 vector、
// 自己造的类型、往 vector 里塞对象、范围 for 走一遍。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-roster.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.1-roster.in

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
    int n;
    std::cin >> n;

    std::vector<Student> roster;

    for (int i = 0; i < n; i++) {
        std::string name;
        int score;
        std::cin >> name >> score;

        roster.push_back(Student(name, score));
    }

    int total = 0;

    for (Student& s : roster) {
        std::cout << s.name << " " << s.score << std::endl;
        total = total + s.score;
    }

    std::cout << "平均 " << total / n << std::endl;

    return 0;
}
