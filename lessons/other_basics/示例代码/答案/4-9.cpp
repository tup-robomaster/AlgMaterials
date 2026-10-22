// 4-9 判断向量组是否线性无关
// 把向量按行排成矩阵，秩等于向量个数就线性无关，否则线性相关。
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using Mat = std::vector<std::vector<double>>;

const double EPS = 1e-9;

int rank_of(Mat A) {
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
        for (int i = r + 1; i < rows; i++) {
            if (std::abs(A[i][c]) > EPS) {
                double k = A[i][c];
                for (int j = c; j < cols; j++) {
                    A[i][j] -= k * A[r][j];
                }
            }
        }
        r++;
    }
    return r;
}

int main() {
    int k, n;
    std::cin >> k >> n;
    Mat M(k, std::vector<double>(n));
    for (auto& row : M) {
        for (double& x : row) {
            std::cin >> x;
        }
    }

    int r = rank_of(M);
    std::cout << "秩 = " << r << std::endl;
    std::cout << (r == k ? "线性无关" : "线性相关") << std::endl;
    return 0;
}
