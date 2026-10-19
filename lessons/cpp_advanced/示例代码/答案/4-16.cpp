// 练习 4-16 的答案：大于 k 的，从大到小。
//
// 两件事分开做，各用各的写法：
//   挑：一条 if 就够，挑进一个新 vector——这一课的工具只回答
//       「有没有、有几个」，不负责把元素挑选出来。
//   排：sort 收第三个参数，那条规矩就是 lambda，a > b 是从大到小。

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> v;
    for (int i = 0; i < n; i++) {
        int x = 0;
        std::cin >> x;
        v.push_back(x);
    }

    int k = 0;
    std::cin >> k;

    std::vector<int> big;
    for (int x : v) {
        if (x > k) {
            big.push_back(x);
        }
    }

    std::sort(big.begin(), big.end(), [](int a, int b) { return a > b; });

    std::cout << "大于 " << k << " 的，从大到小：";
    for (int x : big) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
