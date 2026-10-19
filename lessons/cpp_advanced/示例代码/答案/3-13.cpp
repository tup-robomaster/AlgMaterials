// 练习 3-13 的答案。map 的 key 不一定是 string——这里存放的是 int，
// 输出的顺序就是数值的大小顺序，不是字符串的顺序。

#include <iostream>
#include <map>

int main() {
    int n;
    std::cin >> n;

    std::map<int, int> times;

    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;

        times[x]++;
    }

    for (const auto& kv : times) {
        std::cout << kv.first << " 出现了 " << kv.second << " 次" << std::endl;
    }

    return 0;
}
