// 4-15 复合两个变换
// 每个变换用齐次矩阵表示；先做 T1 再做 T2，合起来是 T2·T1。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Mat = std::vector<std::vector<double>>;

const double PI = 3.14159265358979323846;

Mat trans(double deg, double tx, double ty) {
    double t = deg * PI / 180.0;
    double c = std::cos(t);
    double s = std::sin(t);
    return {{c, -s, tx}, {s, c, ty}, {0.0, 0.0, 1.0}};
}

Mat mul(const Mat& A, const Mat& B) {
    Mat C(3, std::vector<double>(3, 0.0));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

std::vector<double> apply_point(const Mat& T, const std::vector<double>& p) {
    return {T[0][0] * p[0] + T[0][1] * p[1] + T[0][2],
            T[1][0] * p[0] + T[1][1] * p[1] + T[1][2]};
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

int main() {
    double d1, tx1, ty1, d2, tx2, ty2, x, y;
    std::cin >> d1 >> tx1 >> ty1 >> d2 >> tx2 >> ty2 >> x >> y;

    Mat T1 = trans(d1, tx1, ty1);
    Mat T2 = trans(d2, tx2, ty2);
    std::vector<double> q = apply_point(mul(T2, T1), {x, y});

    std::cout << "[" << f(q[0]) << ", " << f(q[1]) << "]" << std::endl;
    return 0;
}
