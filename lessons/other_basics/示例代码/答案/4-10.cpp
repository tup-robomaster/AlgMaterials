// 4-10 求转置
// 转置把第 i 行第 j 列换到第 j 行第 i 列。
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
    int m, n;
    std::cin >> m >> n;
    std::vector<std::vector<double>> M(m, std::vector<double>(n));
    for (auto& row : M) {
        for (double& x : row) {
            std::cin >> x;
        }
    }

    std::vector<std::vector<double>> T(n, std::vector<double>(m));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }
    std::cout << show(T) << std::endl;
    return 0;
}
