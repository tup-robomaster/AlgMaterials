// 练习 4-14 的答案：第一个不及格的。
//
// 4.5 例题里那句是 x >= 60（第一个及格的），这儿反过来写 x < 60，
// 其余部分照写：条件放进方括号后面的圆括号，find_if 用条件遍历一遍。
// 走到头还没找着，it 就是 end()——这时候别加星号，
// 得先说清楚「全都及格了」。

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

    auto it = std::find_if(score.begin(), score.end(),
                           [](int x) { return x < 60; });

    if (it == score.end()) {
        std::cout << "全都及格了" << std::endl;
    } else {
        std::cout << "第一个不及格的是 " << *it << std::endl;
        std::cout << "它在第 " << (it - score.begin()) << " 个" << std::endl;
    }

    return 0;
}
