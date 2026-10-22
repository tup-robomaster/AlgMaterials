// 4-22 求两个奇异值
// 奇异值是 AᵀA 的两个特征值开平方，降序输出。
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

    std::cout << "奇异值 = " << std::fixed << std::setprecision(3) << s1
              << ", " << s2 << std::endl;
    return 0;
}
