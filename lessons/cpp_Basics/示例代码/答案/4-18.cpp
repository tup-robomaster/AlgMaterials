// 练习 4-18 的答案：把 4.3 那道「找最大值」改用 vector 写。
//
// 比较那几行一个字没改。改的只有「怎么开、怎么装」：
// 数组要写死个数，vector 用 push_back 一个一个塞。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> a;

    a.push_back(7);
    a.push_back(0);
    a.push_back(5);
    a.push_back(2);
    a.push_back(9);

    int maxScore = a[0];

    for (int i = 1; i < int(a.size()); i++) {
        if (a[i] > maxScore) {
            maxScore = a[i];
        }
    }

    std::cout << maxScore << std::endl;

    return 0;
}
