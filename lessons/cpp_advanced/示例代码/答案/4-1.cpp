// 练习 4-1 的答案：一层循环从 1 加到 n，同时把执行次数实际统计一遍。
//
// 「执行了 100 次」不是把 n 直接输出出来，而是每次循环加一得到的——
// 这一课要掌握的正是这一点：可以直接看出它执行 n 次，不需要实际逐次数一遍。
// 两种写法的时间复杂度：循环版 O(n)，公式版 n*(n+1)/2 是 O(1)。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int sum = 0;
    int round = 0;
    for (int i = 1; i <= n; i++) {
        sum = sum + i;
        round = round + 1;
    }

    std::cout << "1 加到 " << n << " = " << sum << std::endl;
    std::cout << "循环执行了 " << round << " 次" << std::endl;

    return 0;
}
