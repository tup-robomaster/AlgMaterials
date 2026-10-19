// 练习 3-6 的答案。find 查找等号的位置，substr 从两边各截取一段。

#include <iostream>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    int p = int(line.find('='));

    std::string key = line.substr(0, p);
    std::string value = line.substr(p + 1);

    std::cout << "键：" << key << std::endl;
    std::cout << "值：" << value << std::endl;

    return 0;
}
