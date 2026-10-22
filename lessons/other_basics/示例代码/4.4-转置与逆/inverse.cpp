// 4.4：转置、逆矩阵与正交矩阵。
//
// 2×2 的逆用公式 [[d, -b], [-c, a]] 除以行列式；3×3 把 A 与单位矩阵
// 并排做高斯—约当消元。旋转矩阵满足 RᵀR = I，所以它的逆就是转置。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Mat = std::vector<std::vector<double>>;

const double EPS = 1e-9;

Mat transpose(const Mat& M) {
    Mat T(M[0].size(), std::vector<double>(M.size(), 0.0));
    for (std::size_t i = 0; i < M.size(); i++) {
        for (std::size_t j = 0; j < M[0].size(); j++) {
            T[j][i] = M[i][j];
        }
    }
    return T;
}

Mat inverse(const Mat& M) {
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
            return {};
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
    Mat inv(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = A[i][n + j];
        }
    }
    return inv;
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

int main() {
    Mat A2 = {{2.0, 1.0}, {1.0, 3.0}};
    Mat A3 = {{2.0, 1.0, 0.0}, {1.0, 2.0, 1.0}, {0.0, 1.0, 2.0}};
    Mat R = {{0.0, -1.0}, {1.0, 0.0}};
    Mat RtR = {{0.0, 0.0}, {0.0, 0.0}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                RtR[i][j] += R[k][i] * R[k][j];
            }
        }
    }

    std::cout << "A2 的逆：" << std::endl;
    std::cout << show(inverse(A2)) << std::endl;
    std::cout << "A3 的逆：" << std::endl;
    std::cout << show(inverse(A3)) << std::endl;
    std::cout << "RᵀR = " << show(RtR) << std::endl;
    return 0;
}
