// 2.1 例题：判断一个数是奇数还是偶数。
//
// 用的是 1.3 学的取余：能被 2 整除就是偶数。

#include <iostream>

int main() {
    int n = 7;

    if (n % 2 == 0) {
        std::cout << n << " 是偶数" << std::endl;
    } else {
        std::cout << n << " 是奇数" << std::endl;
    }

    return 0;
}
