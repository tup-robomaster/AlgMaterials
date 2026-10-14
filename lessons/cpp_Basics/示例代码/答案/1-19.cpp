// 练习 1-19 —— 读一个秒数，换算成几分几秒

#include <iostream>

int main() {
    int total;
    std::cin >> total;

    int m = total / 60;
    int s = total % 60;

    std::cout << total << " 秒 = " << m << " 分 " << s << " 秒" << std::endl;

    return 0;
}
