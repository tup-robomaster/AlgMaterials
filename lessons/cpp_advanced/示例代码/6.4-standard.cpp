// 6.4：标准库自己抛出的异常，都从 std::exception 接住。
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30};

    try {
        std::cout << v.at(5) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "at 出错：" << e.what() << std::endl;
    }

    try {
        int x = std::stoi("abc");
        std::cout << x << std::endl;
    } catch (const std::exception& e) {
        std::cout << "stoi 出错：" << e.what() << std::endl;
    }

    return 0;
}
