// 练习 2-1：判断一个整数是正数、负数还是零。
//
// 三档，所以是 if / else if / else。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    if (n > 0) {
        std::cout << "正数" << std::endl;
    } else if (n < 0) {
        std::cout << "负数" << std::endl;
    } else {
        std::cout << "零" << std::endl;
    }

    return 0;
}
