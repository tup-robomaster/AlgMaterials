// 1.5 补充：读进来的是什么，由盒子说了算
//
// 你敲的是 3.7，是个小数。但这个盒子是 int——它只认得下整数那一段。
// 跑的时候输入 3.7 试试。

#include <iostream>

int main() {
    int n;

    std::cin >> n;
    std::cout << n << std::endl;

    return 0;
}
