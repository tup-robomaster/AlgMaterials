// 练习 6-9：把每个词转成整数，分别捕获两种转换错误。
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    std::string s;
    while (std::cin >> s) {
        try {
            int x = std::stoi(s);
            std::cout << s << " -> " << x << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << s << " 不是数字" << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << s << " 超出 int 范围" << std::endl;
        }
    }
    return 0;
}
