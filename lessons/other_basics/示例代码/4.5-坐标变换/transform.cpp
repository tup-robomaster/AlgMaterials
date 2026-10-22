// 4.5：旋转、平移、复合与逆变换。
//
// 平面上的刚体变换用 3×3 齐次矩阵表示：左上 2×2 是旋转，最后一列
// 前两行是平移。点的齐次坐标写成 [x, y, 1]。

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

Mat inverse(const Mat& T) {
    Mat Rt = {{T[0][0], T[1][0], 0.0}, {T[0][1], T[1][1], 0.0},
              {0.0, 0.0, 1.0}};
    double tx = T[0][2];
    double ty = T[1][2];
    Rt[0][2] = -(Rt[0][0] * tx + Rt[0][1] * ty);
    Rt[1][2] = -(Rt[1][0] * tx + Rt[1][1] * ty);
    return Rt;
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
    std::vector<double> p = {1.0, 0.0};
    Mat R90 = trans(90.0, 0.0, 0.0);
    Mat Tt = trans(0.0, 2.0, 1.0);
    Mat T1 = mul(Tt, R90);
    Mat T2 = trans(180.0, 1.0, 0.0);

    std::cout << "旋转 90° = " << show(apply_point(R90, p)) << std::endl;
    std::cout << "旋转后平移 = " << show(apply_point(T1, p)) << std::endl;
    std::cout << "复合矩阵作用于同一点 = " << show(apply_point(mul(Tt, R90), p))
              << std::endl;
    std::cout << "逆变换送回 = "
              << show(apply_point(inverse(T1), apply_point(T1, p))) << std::endl;
    std::cout << "先 T1 再 T2 = " << show(apply_point(mul(T2, T1), p))
              << std::endl;
    return 0;
}
