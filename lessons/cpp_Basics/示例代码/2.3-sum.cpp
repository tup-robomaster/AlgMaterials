// 2.3：1 加到 100，用「攒」的办法。
//
// sum 一开始是 0，每转一圈就把它攒的那个数加进去。

#include <iostream>

int main() {
    int i = 1;
    int sum = 0;

    while (i <= 100) {
        sum = sum + i;
        i = i + 1;
    }

    std::cout << sum << std::endl;

    return 0;
}
