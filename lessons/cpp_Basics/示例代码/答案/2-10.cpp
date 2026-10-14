// 练习 2-10：读一个 n，打出 n 的阶乘。
//
// 攒法和 2.3 的求和一个道理，只是这回攒的是乘出来的东西，
// 所以那个盒子一开始得是 1，不能是 0。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int i = 1;
    int fact = 1;

    while (i <= n) {
        fact = fact * i;
        i = i + 1;
    }

    std::cout << n << "! = " << fact << std::endl;

    return 0;
}
