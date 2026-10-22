// 4-17 求较大特征值与单位特征向量
// 对称矩阵的两个特征值都是实数；特征向量取第一个非零分量为正。
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

    double t = a + d;
    double det = a * d - b * c;
    double s = std::sqrt(std::max(t * t - 4.0 * det, 0.0));
    double lam = (t + s) / 2.0;

    std::vector<double> v;
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

    std::cout << "较大特征值 = " << f(lam) << std::endl;
    std::cout << "单位特征向量 = [" << f(v[0]) << ", " << f(v[1]) << "]"
              << std::endl;
    return 0;
}
