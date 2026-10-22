// 4-8 判断方程组解的情况
// 增广矩阵每行最后一个是等号右边的常数。
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Mat = std::vector<std::vector<double>>;

const double EPS = 1e-9;

void rref(Mat& A) {
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
        r++;
    }
}

int main() {
    int m, n;
    std::cin >> m >> n;
    Mat M(m, std::vector<double>(n + 1));
    for (auto& row : M) {
        for (double& x : row) {
            std::cin >> x;
        }
    }

    Mat R = M;
    rref(R);
    for (const auto& row : R) {
        bool zero = true;
        for (int j = 0; j < n; j++) {
            if (std::abs(row[j]) > EPS) {
                zero = false;
            }
        }
        if (zero && std::abs(row[n]) > EPS) {
            std::cout << "无解" << std::endl;
            return 0;
        }
    }

    int rank_a = 0;
    for (const auto& row : R) {
        for (int j = 0; j < n; j++) {
            if (std::abs(row[j]) > EPS) {
                rank_a++;
                break;
            }
        }
    }
    std::cout << (rank_a < n ? "无穷多解" : "唯一解") << std::endl;
    return 0;
}
