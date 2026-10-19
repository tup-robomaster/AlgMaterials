// 练习 4-11 的答案：最高分是几分，在第几个。
//
// max_element 给的是位置，不是分数——这是它最容易记错的地方。
// 两个问题都由这一个位置得到答案：
//   减一下 begin()，得到的是「第几个」；
//   加一个星号，得到的是那个分数。

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> score;
    for (int i = 0; i < n; i++) {
        int x = 0;
        std::cin >> x;
        score.push_back(x);
    }

    auto it = std::max_element(score.begin(), score.end());

    std::cout << "最高分是 " << *it << std::endl;
    std::cout << "它在第 " << (it - score.begin()) << " 个" << std::endl;

    return 0;
}
