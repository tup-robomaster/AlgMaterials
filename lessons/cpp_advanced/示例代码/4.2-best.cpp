// 4.2 例题：最高分在谁身上——「记住一个位置」到底有什么用。
//
// 从头走到尾找最大的，顺便把「最大的那个在哪儿」记下来。
// 记下来的东西就是一个迭代器。有了它，分数、名次、名字三样都拿得到：
//   best->second          最高分是多少
//   best - roster.begin() 它在第几个（从 0 数）
//   best->first           是谁
// 第 3 部分那些练习里「先找位置再动手」的活，正式的做法就是它。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.2-best.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::pair<std::string, int>> roster = {{"小李", 90},
                                                       {"小王", 77},
                                                       {"小张", 95},
                                                       {"小赵", 85}};

    auto best = roster.begin();
    for (auto it = roster.begin(); it != roster.end(); ++it) {
        if (it->second > best->second) {
            best = it;
        }
    }

    std::cout << "最高分是 " << best->second << std::endl;
    std::cout << "他是 " << best->first << "（第 " << (best - roster.begin())
              << " 个）" << std::endl;

    return 0;
}
