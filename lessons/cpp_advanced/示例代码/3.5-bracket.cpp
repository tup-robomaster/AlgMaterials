// 3.5：例题——括号配不配对，用 stack 一遍扫完。
//
// 这是 stack 最经典的一道题，也是它真正好用的地方：
// 后开的括号得先关上，正好就是「后进先出」。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.5-bracket.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.5-bracket.in

#include <iostream>
#include <stack>
#include <string>

bool matched(const std::string& line) {
    std::stack<char> open;          // 存着还没关上的左括号

    for (char c : line) {
        if (c == '(' || c == '[') {
            open.push(c);
        } else if (c == ')' || c == ']') {
            if (open.empty()) {
                return false;       // 还没开就关，多了一个
            }
            char left = open.top();
            open.pop();
            if (c == ')' && left != '(') {
                return false;       // 关错了种
            }
            if (c == ']' && left != '[') {
                return false;
            }
        }
    }

    return open.empty();            // 若还有剩余，说明有左括号未闭合
}

int main() {
    int n;
    std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);

        std::cout << line << " -> ";
        if (matched(line)) {
            std::cout << "配对" << std::endl;
        } else {
            std::cout << "不配对" << std::endl;
        }
    }

    return 0;
}
