// 练习 4-5 的答案：最低分在第几个。
//
// 跟 4.2 那个「第一个及格的在哪儿」是同一个套路，只是比的方向反过来：
// 记住位置，一路比过去，谁比手里这个最小就换成谁。
// 用 < 而不是 <=，所以并列的时候记住的是最先出现的那个——
// 这题里 62 出现两次，答案给的是第 1 个。

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

    int pos = 0;
    for (int i = 1; i < int(score.size()); i++) {
        if (score[i] < score[pos]) {
            pos = i;
        }
    }

    std::cout << "最低分是 " << score[pos] << std::endl;
    std::cout << "它在第 " << pos << " 个（从 0 数），是第 " << pos + 1
              << " 个输入的（从 1 数）" << std::endl;

    return 0;
}
