// 4-12 3×3 求逆
// 把 A 和单位矩阵并排，做高斯—约当消元；左半边变成单位矩阵时，右半边就是逆。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Mat = std::vector<std::vector<double>>;

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

std::string show(const Mat& M) {
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

bool inverse(const Mat& M, Mat& inv) {
    int n = static_cast<int>(M.size());
    Mat A(n, std::vector<double>(2 * n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = M[i][j];
        }
        A[i][n + i] = 1.0;
    }
    for (int c = 0; c < n; c++) {
        int p = -1;
        for (int i = c; i < n; i++) {
            if (std::abs(A[i][c]) > EPS) {
                p = i;
                break;
            }
        }
        if (p < 0) {
            return false;
        }
        std::swap(A[c], A[p]);
        double v = A[c][c];
        for (double& x : A[c]) {
            x /= v;
        }
        for (int i = 0; i < n; i++) {
            if (i != c && std::abs(A[i][c]) > EPS) {
                double k = A[i][c];
                for (int j = 0; j < 2 * n; j++) {
                    A[i][j] -= k * A[c][j];
                }
            }
        }
    }
    inv.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = A[i][n + j];
        }
    }
    return true;
}

int main() {
    Mat M(3, std::vector<double>(3));
    for (auto& row : M) {
        for (double& x : row) {
            std::cin >> x;
        }
    }

    Mat inv;
    if (!inverse(M, inv)) {
        std::cout << "不可逆" << std::endl;
    } else {
        std::cout << show(inv) << std::endl;
    }
    return 0;
}
