// 练习 0-3 的答案：五个成绩的总分和平均分。
// 两处容易出错的地方都在这一题里：
//   一、vector 得先创建五个元素，再逐个读入——vector 不会自动增加元素。
//   二、平均分那一除，除数得写成 5.0。写 5 的话两边都是整数，
//       432 / 5 得 86，小数部分直接丢弃（基础课 1.3 讲过这一点）。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> scores(5);
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        std::cin >> scores[i];
        sum = sum + scores[i];
    }

    std::cout << "总分 " << sum << std::endl;
    std::cout << "平均 " << sum / 5.0 << std::endl;
    return 0;
}
