// 练习 1-18 —— 读两个词，中间用竖线隔开打出来

#include <iostream>
#include <string>

int main() {
    std::string first;
    std::string second;

    std::cin >> first >> second;
    std::cout << first << "|" << second << std::endl;

    return 0;
}
