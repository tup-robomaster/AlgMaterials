// 4-5 3×3 行列式
// 按第一行展开，三个 2×2 行列式用对角线法则。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const double EPS = 1e-9;

double det2(const std::vector<std::vector<double>>& M) {
    return M[0][0] * M[1][1] - M[0][1] * M[1][0];
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

std::string num(double x) {
    if (std::abs(x - std::round(x)) < EPS) {
        return std::to_string(static_cast<long long>(std::llround(x)));
    }
    return f(x);
}

int main() {
    std::vector<std::vector<double>> M(3, std::vector<double>(3));
    for (auto& row : M) {
        for (double& x : row) {
            std::cin >> x;
        }
    }
    double det = M[0][0] * det2({{M[1][1], M[1][2]}, {M[2][1], M[2][2]}})
               - M[0][1] * det2({{M[1][0], M[1][2]}, {M[2][0], M[2][2]}})
               + M[0][2] * det2({{M[1][0], M[1][1]}, {M[2][0], M[2][1]}});

    std::cout << "行列式 = " << num(det) << std::endl;
    return 0;
}
