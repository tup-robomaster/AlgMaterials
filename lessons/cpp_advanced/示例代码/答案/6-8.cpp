// 练习 6-8：检查年龄，越界时抛出 std::out_of_range。
#include <iostream>
#include <stdexcept>

void checkAge(int age) {
    if (age < 0 || age > 150) {
        throw std::out_of_range("年龄不在 0 到 150 之间");
    }
    std::cout << "年龄 " << age << " 有效" << std::endl;
}

int main() {
    int age = 0;
    while (std::cin >> age) {
        try {
            checkAge(age);
        } catch (const std::out_of_range& e) {
            std::cout << "拒绝：" << e.what() << std::endl;
        }
    }
    return 0;
}
