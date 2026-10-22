// 4.2：2×2 与 3×3 行列式，以及行列式的面积意义。
//
// 3×3 按第一行展开，每一项是一个 2×2 行列式。结果接近整数时按整数
// 打印；否则保留 3 位小数。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Mat = std::vector<std::vector<double>>;

double det2(const Mat& M) {
    return M[0][0] * M[1][1] - M[0][1] * M[1][0];
}

double det3(const Mat& M) {
    Mat m1 = {{M[1][1], M[1][2]}, {M[2][1], M[2][2]}};
    Mat m2 = {{M[1][0], M[1][2]}, {M[2][0], M[2][2]}};
    Mat m3 = {{M[1][0], M[1][1]}, {M[2][0], M[2][1]}};
    return M[0][0] * det2(m1) - M[0][1] * det2(m2) + M[0][2] * det2(m3);
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

std::string num(double x) {
    if (std::abs(x - std::round(x)) < 1e-9) {
        return std::to_string(static_cast<long long>(std::llround(x)));
    }
    return f(x);
}

int main() {
    Mat A2 = {{2.0, 1.0}, {1.0, 3.0}};
    Mat A3 = {{2.0, 1.0, 0.0}, {1.0, 3.0, 1.0}, {0.0, 1.0, 2.0}};

    double px = 0.0, py = 0.0;
    double qx = 4.0, qy = 0.0;
    double rx = 0.0, ry = 3.0;
    double area = std::abs((qx - px) * (ry - py) - (qy - py) * (rx - px)) / 2.0;

    std::cout << "2×2 行列式 = " << num(det2(A2)) << std::endl;
    std::cout << "3×3 行列式 = " << num(det3(A3)) << std::endl;
    std::cout << "三角形面积 = " << f(area) << std::endl;
    return 0;
}
