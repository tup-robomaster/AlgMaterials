// 4-3 矩阵乘向量
// 结果的第 i 个分量是 A 的第 i 行与 x 的内积。
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

std::string show(const std::vector<double>& v) {
    std::string s = "[";
    for (std::size_t i = 0; i < v.size(); i++) {
        if (i > 0) {
            s += ", ";
        }
        s += f(v[i]);
    }
    return s + "]";
}

int main() {
    int m, n;
    std::cin >> m >> n;
    std::vector<std::vector<double>> A(m, std::vector<double>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> A[i][j];
        }
    }
    std::vector<double> x(n);
    for (double& v : x) {
        std::cin >> v;
    }

    std::vector<double> y(m, 0.0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
    std::cout << show(y) << std::endl;
    return 0;
}
