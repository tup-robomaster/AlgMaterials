// 4.4：数、找最大最小、倒过来——都是现成的，一句话一个。
//
// 这一份里没有一个循环：以前要写五六行的内容，
// 现在传入两个迭代器即可完成。节省下来的不是打字量，而是出错的机会。
//
// 那两个取最大最小的函数返回的也是迭代器，所以要加一个 * 才能取得数值——
// 跟 4.2 里 *it 是同一个道理。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.4-min-max.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    std::cout << "最高的 " << *std::max_element(score.begin(), score.end())
              << " 分" << std::endl;
    std::cout << "最低的 " << *std::min_element(score.begin(), score.end())
              << " 分" << std::endl;

    std::cout << "68 出现了 "
              << std::count(score.begin(), score.end(), 68) << " 次" << std::endl;
    std::cout << "60 出现了 "
              << std::count(score.begin(), score.end(), 60) << " 次" << std::endl;

    std::reverse(score.begin(), score.end());
    std::cout << "倒过来：";
    for (int x : score) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
