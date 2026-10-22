// 4-23 最佳秩 1 近似
// 取最大奇异值 σ1 和对应的单位向量 u1、v1，近似矩阵是 σ1·u1·v1ᵀ。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string f(double x) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(3) << x;
    std::string s = os.str();
    if (s == "-0.000") {
        s = "0.000";
    }
    return s;
}

int main() {
    double a, b, c, d;
    std::cin >> a >> b >> c >> d;

    double p = a * a + c * c;
    double q = a * b + c * d;
    double r = b * b + d * d;
    double t = p + r;
    double det = p * r - q * q;
    double disc = std::sqrt(std::max(t * t - 4.0 * det, 0.0));
    double l1 = (t + disc) / 2.0;
    double s1 = std::sqrt(std::max(l1, 0.0));

    std::vector<std::vector<double>> R(2, std::vector<double>(2, 0.0));
    if (s1 != 0.0) {
        std::vector<double> v;
        if (std::abs(q) > 1e-12) {
            v = {q, l1 - p};
        } else if (p >= r) {
            v = {1.0, 0.0};
        } else {
            v = {0.0, 1.0};
        }
        double n = std::sqrt(v[0] * v[0] + v[1] * v[1]);
        v[0] /= n;
        v[1] /= n;
        std::vector<double> w = {a * v[0] + b * v[1], c * v[0] + d * v[1]};
        std::vector<double> u = {w[0] / s1, w[1] / s1};
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                R[i][j] = s1 * u[i] * v[j];
            }
        }
    }

    std::cout << "[[" << f(R[0][0]) << ", " << f(R[0][1]) << "], ["
              << f(R[1][0]) << ", " << f(R[1][1]) << "]]" << std::endl;
    return 0;
}
