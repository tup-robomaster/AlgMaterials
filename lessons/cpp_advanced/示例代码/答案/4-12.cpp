// 练习 4-12 的答案：大于 60 的有几个，是哪几个。
//
// 现成的工具里没有「大于 60 的」这种说法——count 只认「等于」，
// 所以这两件事都自己走一趟：先数，再挑。
// 挑出来的那份从小到大排一下，用 sort 就一句话。
// 下一课学的 count_if 能把「数」那一趟也省掉，先手写一遍，
// 到时候才知道它省掉的是什么。

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

    int count = 0;
    for (int x : score) {
        if (x > 60) {
            count = count + 1;
        }
    }

    std::vector<int> pass;
    for (int x : score) {
        if (x > 60) {
            pass.push_back(x);
        }
    }

    std::sort(pass.begin(), pass.end());

    std::cout << "大于 60 的有 " << count << " 个：";
    for (int x : pass) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
