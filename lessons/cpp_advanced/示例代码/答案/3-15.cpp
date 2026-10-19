// 练习 3-15 的答案。倒序输出有一个更简洁的写法：全部压入 stack，
// 再逐个取栈顶——取出的顺序自然就是倒序。

#include <iostream>
#include <stack>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    std::stack<char> s;

    for (char c : line) {
        s.push(c);
    }

    while (!s.empty()) {
        std::cout << s.top();
        s.pop();
    }
    std::cout << std::endl;

    return 0;
}
