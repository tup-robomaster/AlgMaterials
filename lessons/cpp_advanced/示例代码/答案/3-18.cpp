// 练习 3-18 的答案。一份数据灌进两个容器，两个容器各回答一个问题。
// 正序输出是 queue 的用途，倒序输出是 stack 的用途。

#include <iostream>
#include <queue>
#include <stack>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    std::queue<char> q;
    std::stack<char> s;

    for (char c : line) {
        q.push(c);
        s.push(c);
    }

    std::cout << "正序输出：";
    while (!q.empty()) {
        std::cout << q.front();
        q.pop();
    }
    std::cout << std::endl;

    std::cout << "倒序输出：";
    while (!s.empty()) {
        std::cout << s.top();
        s.pop();
    }
    std::cout << std::endl;

    return 0;
}
