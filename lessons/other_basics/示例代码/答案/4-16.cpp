// 4-16 求 2×2 实特征值
// 特征方程 λ² - (a+d)λ + (ad-bc) = 0，判别式小于 0 时没有实特征值。
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    double a, b, c, d;
    std::cin >> a >> b >> c >> d;

    double t = a + d;
    double det = a * d - b * c;
    double disc = t * t - 4.0 * det;
    if (disc < -1e-9) {
        std::cout << "无实特征值" << std::endl;
        return 0;
    }
    double s = std::sqrt(std::max(disc, 0.0));
    double l1 = (t + s) / 2.0;
    double l2 = (t - s) / 2.0;
    if (l1 < l2) {
        std::swap(l1, l2);
    }
    std::cout << "特征值 = " << std::fixed << std::setprecision(3) << l1
              << ", " << l2 << std::endl;
    return 0;
}
