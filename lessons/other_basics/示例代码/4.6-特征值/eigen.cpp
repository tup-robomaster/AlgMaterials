// 4.6：2×2 特征值、特征向量与幂迭代。
//
// 特征方程是 λ² - (a+d)λ + (ad-bc) = 0。对称矩阵的两个特征向量正交，
// Q Λ Qᵀ 能还原原矩阵。幂迭代每步乘一次 A 并归一化，用瑞利商估计主特征值。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Vec = std::vector<double>;
using Mat = std::vector<Vec>;

std::vector<double> eig2(const Mat& M) {
    double a = M[0][0], b = M[0][1];
    double c = M[1][0], d = M[1][1];
    double t = a + d;
    double det = a * d - b * c;
    double s = std::sqrt(std::max(t * t - 4.0 * det, 0.0));
    return {(t + s) / 2.0, (t - s) / 2.0};
}

Vec eigvec2(const Mat& M, double lam) {
    double a = M[0][0], b = M[0][1];
    double c = M[1][0], d = M[1][1];
    Vec v;
    if (std::abs(b) > 1e-12) {
        v = {b, lam - a};
    } else if (std::abs(c) > 1e-12) {
        v = {lam - d, c};
    } else if (std::abs(a - lam) < 1e-12) {
        v = {1.0, 0.0};
    } else {
        v = {0.0, 1.0};
    }
    double n = std::sqrt(v[0] * v[0] + v[1] * v[1]);
    v[0] /= n;
    v[1] /= n;
    for (double x : v) {
        if (std::abs(x) > 1e-12) {
            if (x < 0.0) {
                v[0] = -v[0];
                v[1] = -v[1];
            }
            break;
        }
    }
    return v;
}

double power(const Mat& A, Vec v, int steps = 100) {
    double lam = 0.0;
    for (int step = 0; step < steps; step++) {
        Vec w(v.size(), 0.0);
        for (std::size_t i = 0; i < v.size(); i++) {
            for (std::size_t j = 0; j < v.size(); j++) {
                w[i] += A[i][j] * v[j];
            }
        }
        double vv = 0.0;
        double vw = 0.0;
        for (std::size_t i = 0; i < v.size(); i++) {
            vv += v[i] * v[i];
            vw += v[i] * w[i];
        }
        lam = vw / vv;
        double n = 0.0;
        for (double x : w) {
            n += x * x;
        }
        n = std::sqrt(n);
        for (double& x : w) {
            x /= n;
        }
        v = w;
    }
    return lam;
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
    Mat A = {{2.0, 1.0}, {1.0, 2.0}};
    std::vector<double> lams = eig2(A);
    Vec v1 = eigvec2(A, lams[0]);
    Vec v2 = eigvec2(A, lams[1]);
    Mat Q = {{v1[0], v2[0]}, {v1[1], v2[1]}};
    Mat QlQ = {{0.0, 0.0}, {0.0, 0.0}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                QlQ[i][j] += Q[i][k] * lams[k] * Q[j][k];
            }
        }
    }
    Mat B = {{4.0, 1.0, 0.0}, {1.0, 3.0, 1.0}, {0.0, 1.0, 2.0}};

    std::cout << "特征值 = " << f(lams[0]) << ", " << f(lams[1])
              << std::endl;
    std::cout << "较大特征值的单位特征向量 = [" << f(v1[0]) << ", "
              << f(v1[1]) << "]" << std::endl;
    std::cout << "Q Λ Qᵀ = [[" << f(QlQ[0][0]) << ", " << f(QlQ[0][1])
              << "], [" << f(QlQ[1][0]) << ", " << f(QlQ[1][1]) << "]]"
              << std::endl;
    std::cout << "幂迭代的主特征值 = " << f(power(B, {1.0, 1.0, 1.0}))
              << std::endl;
    return 0;
}
