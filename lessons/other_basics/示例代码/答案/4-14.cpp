// 4-14 旋转后平移
// 先算 R p，再加上平移量 (tx, ty)。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const double PI = 3.14159265358979323846;

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
    double deg, tx, ty, x, y;
    std::cin >> deg >> tx >> ty >> x >> y;

    double t = deg * PI / 180.0;
    double c = std::cos(t);
    double s = std::sin(t);
    double rx = c * x - s * y + tx;
    double ry = s * x + c * y + ty;

    std::cout << "[" << f(rx) << ", " << f(ry) << "]" << std::endl;
    return 0;
}
