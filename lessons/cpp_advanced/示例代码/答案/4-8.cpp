// 练习 4-8 的答案：一边走一边把偶数删掉。
//
// 这就是 4.3 那个标准形式：erase 把「下一个该查看的位置」返回给你，
// 接住它接着走；没删的时候才自己 ++it。for 的第三格因此空着——
// 填上 ++it 的话，删完会再往前走一格，正好漏掉一个数。

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

    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 == 0) {
            it = v.erase(it);
        } else {
            ++it;
        }
    }

    std::cout << "剩下的奇数：";
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
