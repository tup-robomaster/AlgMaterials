// 4.4 例题：一份成绩单的统计报告。
//
// 最高、最低、平均、排序、中位数——五件事，各用一个现成的工具。
// 中位数放在最后讲解是有原因的：它必须先排序，才能确定「中间那个」，
// 正是 sort 最典型的用法。
//
// 输入：第一行 n，第二行 n 个分数。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.4-report.cpp -o /tmp/t && /tmp/t < 示例代码/4.4-report.in

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> score;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        int x = 0;
        std::cin >> x;
        score.push_back(x);
        sum = sum + x;
    }

    std::cout << "最高 " << *std::max_element(score.begin(), score.end())
              << " 分" << std::endl;
    std::cout << "最低 " << *std::min_element(score.begin(), score.end())
              << " 分" << std::endl;

    std::cout << "平均 " << sum << " / " << n << " = "
              << sum / n << " 分（整数除法，抹掉小数）" << std::endl;

    std::sort(score.begin(), score.end());

    std::cout << "从小到大：";
    for (int x : score) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << "中位数是第 " << n / 2 << " 个（从 0 数）："
              << score[n / 2] << " 分" << std::endl;

    return 0;
}
