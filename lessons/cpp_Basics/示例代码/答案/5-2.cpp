// 练习 5-2 的答案：读进来一张名单。
//
// 读的时候也是三样一起读：cin >> s[i].name >> s[i].id >> s[i].score;
// 每一段之间用空格隔开就行，cin 自己会跳过去。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    int score;
};

int main() {
    int n;
    std::cin >> n;

    Student s[100];

    for (int i = 0; i < n; i++) {
        std::cin >> s[i].name >> s[i].id >> s[i].score;
    }

    for (int i = 0; i < n; i++) {
        std::cout << s[i].name << " " << s[i].id << " " << s[i].score << std::endl;
    }

    return 0;
}
