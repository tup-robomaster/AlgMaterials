// 练习 4-15 的答案：在 10 到 20 之间的有几个、是哪些。
//
// 条件是两个条件用 && 连起来的，写成 lambda 就是一句话。
// count_if 只管「有几个」，要说清「是哪些」，还得自己走一趟——
// 走的时候用的是同一条条件，所以写成一个具名的 lambda，
// 两处共用一份，改的时候只改一处。

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

    auto in_range = [](int x) { return x >= 10 && x <= 20; };

    int count = int(std::count_if(v.begin(), v.end(), in_range));

    std::cout << "在 10 到 20 之间的有 " << count << " 个：";
    for (int x : v) {
        if (in_range(x)) {
            std::cout << x << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
