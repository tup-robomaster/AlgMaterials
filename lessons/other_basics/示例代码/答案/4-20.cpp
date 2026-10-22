// 4-20 过原点拟合
// 直线过原点时 b = 0，只有一个未知量 a = Σxy / Σx²。
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<std::pair<double, double>> pts(n);
    for (auto& p : pts) {
        std::cin >> p.first >> p.second;
    }

    double sxx = 0.0;
    double sxy = 0.0;
    for (const auto& p : pts) {
        sxx += p.first * p.first;
        sxy += p.first * p.second;
    }
    double a = sxy / sxx;

    std::cout << "a = " << std::fixed << std::setprecision(3) << a
              << std::endl;
    return 0;
}
