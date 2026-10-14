// 1.3 例题：把 250 分钟换算成几小时几分钟

#include <iostream>

int main() {
    int total = 250;            // 一共 250 分钟

    int hours   = total / 60;   // 一小时 60 分钟。要「有几个整 60」，用 /
    int minutes = total % 60;   // 剩下的零头，用 %

    std::cout << hours << std::endl;
    std::cout << minutes << std::endl;

    return 0;
}
