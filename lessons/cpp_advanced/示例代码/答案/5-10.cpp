// 练习 5-10 的答案：emplace_back 直接构造，std::move 整批转移。
//
// 这一部分是这一课的收束：添加元素用最节省的方式（当场构造，不复制），
// 转移位置也用最节省的方式（移动，不复制第二份）。
// 移动之后 first 就是空的，接着要使用的是 second。

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Student {
public:
    std::string name;
    int score;

    Student(std::string n, int s) : name(n), score(s) {}
};

int main() {
    std::vector<Student> first;
    first.emplace_back("小李", 90);
    first.emplace_back("小王", 85);

    std::cout << "移动之前：first 有 " << first.size() << " 个" << std::endl;

    std::vector<Student> second = std::move(first);

    std::cout << "移动之后：second 有 " << second.size() << " 个，first 有 "
              << first.size() << " 个" << std::endl;

    for (const Student& s : second) {
        std::cout << s.name << " " << s.score << std::endl;
    }

    return 0;
}
