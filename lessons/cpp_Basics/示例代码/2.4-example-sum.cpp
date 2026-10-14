// 2.4：例题。读一个 n，算 1 加到 n。
//
// 2.3 用 while 写过一遍。这次用 for——攒东西那部分一个字没变，
// 只是「起点、条件、第三步」从三处搬到了头上。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int sum = 0;
    for (int i = 1; i <= n; i = i + 1) {
        sum = sum + i;
    }

    std::cout << sum << std::endl;

    return 0;
}
