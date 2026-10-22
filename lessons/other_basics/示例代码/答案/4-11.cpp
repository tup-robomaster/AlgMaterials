// 4-11 2×2 求逆
// 行列式为 0 时没有逆矩阵；否则用 [[d,-b],[-c,a]] 除以行列式。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const double EPS = 1e-9;

std::string f(double x) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(3) << x;
    std::string s = os.str();
    if (s == "-0.000") {
        s = "0.000";
    }
    return s;
}

std::string show(const std::vector<std::vector<double>>& M) {
    std::string s = "[";
    for (std::size_t i = 0; i < M.size(); i++) {
        if (i > 0) {
            s += ", ";
        }
        s += "[";
        for (std::size_t j = 0; j < M[i].size(); j++) {
            if (j > 0) {
                s += ", ";
            }
            s += f(M[i][j]);
        }
        s += "]";
    }
    return s + "]";
}

int main() {
    double a, b, c, d;
    std::cin >> a >> b >> c >> d;
    double det = a * d - b * c;

    if (std::abs(det) < EPS) {
        std::cout << "不可逆" << std::endl;
        return 0;
    }
    std::vector<std::vector<double>> inv = {
        {d / det, -b / det}, {-c / det, a / det}};
    std::cout << show(inv) << std::endl;
    return 0;
}
