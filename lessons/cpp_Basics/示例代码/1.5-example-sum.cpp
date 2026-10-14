// 1.5 例题：从你手里要两个数，把和打出来
//
// 跑起来它会停住等你。你敲两个数，它才算得下去。

#include <iostream>

int main() {
    int a;
    int b;

    std::cout << "第一个数：" << std::endl;
    std::cin >> a;

    std::cout << "第二个数：" << std::endl;
    std::cin >> b;

    std::cout << "和是 " << a + b << std::endl;

    return 0;
}
