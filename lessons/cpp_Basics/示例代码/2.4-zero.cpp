// 2.4：从 0 数起。
//
// 打 n 次，用 0 到 n-1 数——这是 C++ 里最常用的一种数法，
// 以后遍历数组（4.2）用的就是它。
//
// 数出来是 0 1 2 而不是 1 2 3，但圈数一样是 3 圈。
// 「圈数对不对」和「数出来的值对不对」是两件事，别混。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i = i + 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
