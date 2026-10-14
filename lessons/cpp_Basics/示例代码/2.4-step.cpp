// 2.4：第三格不一定加 1。
//
// 这里每次加 2，打出 1 到 9 的奇数。

#include <iostream>

int main() {
    for (int i = 1; i <= 9; i = i + 2) {
        std::cout << i << std::endl;
    }

    return 0;
}
