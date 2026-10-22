// 4-18 幂迭代求主特征值
// 每步乘一次 A 并归一化，用瑞利商估计特征值；100 步足够收敛。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<double>> A(3, std::vector<double>(3));
    for (auto& row : A) {
        for (double& x : row) {
            std::cin >> x;
        }
    }
    std::vector<double> v(3);
    for (double& x : v) {
        std::cin >> x;
    }

    double lam = 0.0;
    for (int step = 0; step < 100; step++) {
        std::vector<double> w(3, 0.0);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                w[i] += A[i][j] * v[j];
            }
        }
        double vv = 0.0;
        double vw = 0.0;
        for (int i = 0; i < 3; i++) {
            vv += v[i] * v[i];
            vw += v[i] * w[i];
        }
        lam = vw / vv;
        double n = std::sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
        for (double& x : w) {
            x /= n;
        }
        v = w;
    }

    std::cout << "主特征值 = " << std::fixed << std::setprecision(3) << lam
              << std::endl;
    return 0;
}
