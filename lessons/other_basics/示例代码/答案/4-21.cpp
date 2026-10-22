// 4-21 用拟合结果预测
// 先按正规方程求出 a、b，再把 x0 代入 y = a x + b。
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
    double x0;
    std::cin >> x0;

    double sx = 0.0, sy = 0.0, sxx = 0.0, sxy = 0.0;
    for (const auto& p : pts) {
        sx += p.first;
        sy += p.second;
        sxx += p.first * p.first;
        sxy += p.first * p.second;
    }
    double a = (n * sxy - sx * sy) / (n * sxx - sx * sx);
    double b = (sy - a * sx) / n;

    std::cout << "预测值 = " << std::fixed << std::setprecision(3)
              << a * x0 + b << std::endl;
    return 0;
}
