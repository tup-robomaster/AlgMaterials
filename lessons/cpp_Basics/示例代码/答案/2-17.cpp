// 练习 2-17 答案：判断 n 是不是质数。
//
// 三样东西凑一块儿了：for（试每一个可能的约数）、
// if（发现能整除）、bool（记住「找到过」这件事）。
//
// 写法是「先假设它是质数，找到一个约数就推翻这个假设」。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    bool isPrime = true;

    for (int i = 2; i < n; i = i + 1) {
        if (n % i == 0) {
            isPrime = false;
        }
    }

    if (isPrime) {
        std::cout << n << " 是质数" << std::endl;
    } else {
        std::cout << n << " 不是质数" << std::endl;
    }

    return 0;
}
