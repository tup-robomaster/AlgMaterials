#include "stats.h"

#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> scores(n);
    for (int i = 0; i < n; i++) {
        std::cin >> scores[i];
    }

    int tenths = static_cast<int>(average(scores) * 10.0 + 0.5);
    std::cout << "及格人数：" << pass_count(scores, 60) << "\n";
    std::cout << "平均分：" << tenths / 10 << "." << tenths % 10 << "\n";
    std::cout << "最高分：" << max_score(scores) << "\n";
    return 0;
}
