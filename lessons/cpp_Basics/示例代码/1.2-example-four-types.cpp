// 1.2 例题：四种类型各开一个盒子

#include <iostream>

int main() {
    double price = 12.5;        // 单价，有小数，用 double
    int    cups  = 3;           // 杯数，整数，用 int
    char   size  = 'M';         // 一个字母，用 char
    bool   sugar = true;        // 加不加糖，用 bool

    std::cout << price << std::endl;
    std::cout << cups << std::endl;
    std::cout << size << std::endl;
    std::cout << sugar << std::endl;

    return 0;
}
