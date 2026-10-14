// 2.2：&& || ! 三个连词。

#include <iostream>

int main() {
    int score = 85;
    int age = 19;

    if (score >= 60 && age >= 18) {
        std::cout << "两个都满足" << std::endl;
    }

    if (score >= 90 || age >= 18) {
        std::cout << "至少满足一个" << std::endl;
    }

    if (!(score >= 60)) {
        std::cout << "没及格" << std::endl;
    } else {
        std::cout << "及格了" << std::endl;
    }

    return 0;
}
