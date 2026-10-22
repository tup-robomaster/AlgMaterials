// 4-7 求矩阵的秩
// 用高斯消元把矩阵化成行阶梯形，主元的个数就是秩。
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
    int m, n;
    std::cin >> m >> n;
    Mat M(m, std::vector<double>(n));
    for (auto& row : M) {
        for (double& x : row) {
            std::cin >> x;
        }
    }
    std::cout << "秩 = " << rank_of(M) << std::endl;
    return 0;
}
