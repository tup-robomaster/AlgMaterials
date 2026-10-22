// 4-24 求条件数
// 条件数 = σ1 / σ2；较小的奇异值为 0 时，矩阵不可逆，条件数记为无穷。
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    double a, b, c, d;
    std::cin >> a >> b >> c >> d;

    double p = a * a + c * c;
    double q = a * b + c * d;
    double r = b * b + d * d;
    double t = p + r;
    double det = p * r - q * q;
    double disc = std::sqrt(std::max(t * t - 4.0 * det, 0.0));
    double s1 = std::sqrt(std::max((t + disc) / 2.0, 0.0));
    double s2 = std::sqrt(std::max((t - disc) / 2.0, 0.0));

    if (s2 < 1e-12) {
        std::cout << "条件数 = 无穷" << std::endl;
    } else {
        std::cout << "条件数 = " << std::fixed << std::setprecision(3)
                  << s1 / s2 << std::endl;
    }
    return 0;
}
