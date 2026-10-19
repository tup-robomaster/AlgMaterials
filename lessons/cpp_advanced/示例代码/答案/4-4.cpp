// 练习 4-4 的答案：倒着打。
//
// 起点是 v.end()，也就是最后一个位置的后面，所以每次循环先把 it 向前移动一个位置再输出——
// 先退后打，顺序就反过来了。
// 循环条件是 it != v.begin()：走到第一格就停，而这一格正好是最后打的那个。

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

    for (auto it = v.end(); it != v.begin();) {
        --it;
        std::cout << *it << std::endl;
    }

    return 0;
}
