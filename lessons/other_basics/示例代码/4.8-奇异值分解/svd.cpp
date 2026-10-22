// 4.8：2×2 奇异值分解、秩 1 近似与条件数。
//
// 奇异值是 AᵀA 的特征值开平方。取最大奇异值 σ1 和对应的单位向量
// u1、v1，最佳秩 1 近似就是 σ1·u1·v1ᵀ。条件数是 σ1 / σ2。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Mat = std::vector<std::vector<double>>;

std::pair<double, double> svd2(const Mat& M) {
    double a = M[0][0], b = M[0][1];
    double c = M[1][0], d = M[1][1];
    double p = a * a + c * c;
    double q = a * b + c * d;
    double r = b * b + d * d;
    double t = p + r;
    double det = p * r - q * q;
    double disc = std::sqrt(std::max(t * t - 4.0 * det, 0.0));
    double l1 = (t + disc) / 2.0;
    double l2 = (t - disc) / 2.0;
    return {std::sqrt(std::max(l1, 0.0)), std::sqrt(std::max(l2, 0.0))};
}

Mat rank1(const Mat& M) {
    double a = M[0][0], b = M[0][1];
    double c = M[1][0], d = M[1][1];
    double p = a * a + c * c;
    double q = a * b + c * d;
    double r = b * b + d * d;
    double t = p + r;
    double det = p * r - q * q;
    double disc = std::sqrt(std::max(t * t - 4.0 * det, 0.0));
    double l1 = (t + disc) / 2.0;
    double s1 = std::sqrt(std::max(l1, 0.0));
    if (s1 == 0.0) {
        return {{0.0, 0.0}, {0.0, 0.0}};
    }
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
    Mat R(2, std::vector<double>(2, 0.0));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            R[i][j] = s1 * u[i] * v[j];
        }
    }
    return R;
}

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
    Mat A = {{4.0, 0.0}, {3.0, 5.0}};
    std::pair<double, double> s = svd2(A);
    Mat R = rank1(A);

    std::cout << "奇异值 = " << f(s.first) << ", " << f(s.second)
              << std::endl;
    std::cout << "秩 1 近似 = [[" << f(R[0][0]) << ", " << f(R[0][1])
              << "], [" << f(R[1][0]) << ", " << f(R[1][1]) << "]]"
              << std::endl;
    std::cout << "条件数 = " << f(s.first / s.second) << std::endl;
    return 0;
}
