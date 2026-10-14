// 练习 2-9：读一个 n，打出 1 到 n。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int i = 1;
    while (i <= n) {
        std::cout << i << std::endl;
        i = i + 1;
    }

    return 0;
}
