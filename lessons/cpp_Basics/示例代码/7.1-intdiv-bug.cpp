// 练习 7-5 正文里贴的那一份（错）：整数除法。
// 前三行都对，第四行该打 3.5，它打 3。

#include <iostream>

int main() {
    int a = 7;
    int b = 2;

    std::cout << "和：" << a + b << std::endl;
    std::cout << "差：" << a - b << std::endl;
    std::cout << "积：" << a * b << std::endl;
    std::cout << "商：" << a / b << std::endl;

    return 0;
}
