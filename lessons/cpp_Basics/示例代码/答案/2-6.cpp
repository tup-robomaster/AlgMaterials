// 练习 2-6：判断闰年。
//
// 规则：能被 4 整除，且不能被 100 整除的，是闰年；
//       能被 400 整除的，也是闰年。

#include <iostream>

int main() {
    int year = 0;
    std::cin >> year;

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        std::cout << year << " 是闰年" << std::endl;
    } else {
        std::cout << year << " 不是闰年" << std::endl;
    }

    return 0;
}
