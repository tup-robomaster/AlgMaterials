// 4.3：行简化、秩与方程组的解。
//
// 高斯—约当消元把矩阵化成行简化阶梯形，主元的个数就是秩。
// 这里用 double 计算，判断一个数是否为 0 时留一个很小的容差。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Mat = std::vector<std::vector<double>>;

const double EPS = 1e-9;

void rref(Mat& A, std::vector<int>& pivots) {
    int rows = static_cast<int>(A.size());
    int cols = static_cast<int>(A[0].size());
    int r = 0;
    for (int c = 0; c < cols && r < rows; c++) {
        int p = -1;
        for (int i = r; i < rows; i++) {
            if (std::abs(A[i][c]) > EPS) {
                p = i;
                break;
            }
        }
        if (p < 0) {
            continue;
        }
        std::swap(A[r], A[p]);
        double v = A[r][c];
        for (double& x : A[r]) {
            x /= v;
        }
        for (int i = 0; i < rows; i++) {
            if (i != r && std::abs(A[i][c]) > EPS) {
                double k = A[i][c];
                for (int j = 0; j < cols; j++) {
                    A[i][j] -= k * A[r][j];
                }
            }
        }
        pivots.push_back(c);
        r++;
    }
}

std::string num(double x) {
    if (std::abs(x - std::round(x)) < EPS) {
        return std::to_string(static_cast<long long>(std::llround(x)));
    }
    std::ostringstream os;
    os << std::fixed << std::setprecision(3) << x;
    return os.str();
}

std::string show(const Mat& M) {
    std::string s;
    for (std::size_t i = 0; i < M.size(); i++) {
        if (i > 0) {
            s += "\n";
        }
        s += "[";
        for (std::size_t j = 0; j < M[i].size(); j++) {
            if (j > 0) {
                s += ", ";
            }
            s += num(M[i][j]);
        }
        s += "]";
    }
    return s;
}

std::string kind(const Mat& M) {
    Mat A = M;
    std::vector<int> pivots;
    rref(A, pivots);
    int n = static_cast<int>(M[0].size()) - 1;
    for (const auto& row : A) {
        bool zero = true;
        for (int j = 0; j < n; j++) {
            if (std::abs(row[j]) > EPS) {
                zero = false;
            }
        }
        if (zero && std::abs(row[n]) > EPS) {
            return "无解";
        }
    }
    if (static_cast<int>(pivots.size()) < n) {
        return "无穷多解";
    }
    return "唯一解";
}

int main() {
    Mat M = {{1.0, 2.0, 0.0}, {2.0, 4.0, 1.0}, {0.0, 0.0, 1.0}};
    Mat R = M;
    std::vector<int> pivots;
    rref(R, pivots);

    std::cout << "行简化：" << std::endl;
    std::cout << show(R) << std::endl;
    std::cout << "秩 = " << pivots.size() << std::endl;
    std::cout << "增广矩阵的解：" << kind({{1.0, 1.0, 3.0}, {2.0, -1.0, 0.0}})
              << std::endl;
    return 0;
}
