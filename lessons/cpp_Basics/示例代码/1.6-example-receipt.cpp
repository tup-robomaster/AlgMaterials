// 1.6 例题：对齐一张小票
//
// 上半段用 cout 打（1.4 学的），下半段用 printf 打（这一课学的）。
// 数字是一样的。盯住小数点在哪一列。

#include <cstdio>
#include <iostream>

int main() {
    std::cout << "用 cout 打：" << std::endl;
    std::cout << 12.5 << std::endl;
    std::cout << 3.0 << std::endl;
    std::cout << 118.75 << std::endl;

    std::cout << "用 printf 打：" << std::endl;
    printf("%8.2f\n", 12.5);
    printf("%8.2f\n", 3.0);
    printf("%8.2f\n", 118.75);

    return 0;
}
