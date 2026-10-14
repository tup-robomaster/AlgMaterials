// 练习 2-5：判断一个整数在不在 1 到 100 之间。

#include <iostream>

int main() {
    int x = 0;
    std::cin >> x;

    if (x >= 1 && x <= 100) {
        std::cout << "在范围里" << std::endl;
    } else {
        std::cout << "不在范围里" << std::endl;
    }

    return 0;
}
