// 练习 2-3：判断一个整数能不能被 3 整除。
//
// 能不能整除，就是问余数是不是 0——1.3 的取余在这儿派上用场。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    if (n % 3 == 0) {
        std::cout << n << " 能被 3 整除" << std::endl;
    } else {
        std::cout << n << " 不能被 3 整除，余 " << n % 3 << std::endl;
    }

    return 0;
}
