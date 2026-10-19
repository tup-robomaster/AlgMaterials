// 5.5 例题：emplace_back 直接构造，push_back 先创建再放进去。
//
// 3.1 说过「往容器里添加元素，常常是复制一份进去的」——当时看不见，
// 这一课让它显示出来：Student 的拷贝构造函数里有一行打印，复制几次就打印几次。
// 两份各装一个学生：一份先创建、再复制进去（打印两次），
// 一份直接在目标位置构造（打印一次）。
// 两份名单互不相干，避免容器扩容时的复制混进来。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.5-emplace.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <vector>

class Student {
public:
    std::string name;
    int score;

    Student(std::string n, int s) : name(n), score(s) {
        std::cout << "（构造一个 " << name << "）" << std::endl;
    }

    Student(const Student& other) : name(other.name), score(other.score) {
        std::cout << "（复制一份 " << name << "）" << std::endl;
    }
};

int main() {
    std::vector<Student> by_push;

    std::cout << "—— push_back：先创建，再复制进去 ——" << std::endl;
    by_push.push_back(Student("小李", 90));

    std::vector<Student> by_emplace;

    std::cout << "—— emplace_back：直接在目标位置构造 ——" << std::endl;
    by_emplace.emplace_back("小王", 88);

    std::cout << "—— 两份名单 ——" << std::endl;
    for (const Student& s : by_push) {
        std::cout << s.name << " " << s.score << std::endl;
    }
    for (const Student& s : by_emplace) {
        std::cout << s.name << " " << s.score << std::endl;
    }

    return 0;
}
