// 3.3：例题——给 3.1 那张成绩单加上 const。
//
// 跟 3.1-roster.cpp 是同一份程序，只多了三处 const，
// 外加把求平均那段单独拎成一个函数（拎出来才看得出 const 加在哪儿）。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.3-roster-const.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.3-roster-const.in

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

int total(const std::vector<Student>& roster) {
    int sum = 0;

    for (const Student& s : roster) {
        sum = sum + s.score;
    }

    return sum;
}

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

    for (const Student& s : roster) {
        std::cout << s.name << " " << s.score << std::endl;
    }

    std::cout << "平均 " << total(roster) / n << std::endl;

    return 0;
}
