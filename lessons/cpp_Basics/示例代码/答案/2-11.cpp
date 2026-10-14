// 练习 2-11：读一个正整数，数它一共有几位。
//
// 每除一次 10 就少一位，数到它变成 0 为止。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int count = 0;

    while (n > 0) {
        n = n / 10;
        count = count + 1;
    }

    std::cout << count << " 位" << std::endl;

    return 0;
}
