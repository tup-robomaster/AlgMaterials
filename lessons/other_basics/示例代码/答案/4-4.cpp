// 4-4 2×2 行列式与可逆判断
// 行列式不为 0 就可逆；接近 0 时按 0 处理。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const double EPS = 1e-9;

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
    if (std::abs(x - std::round(x)) < EPS) {
        return std::to_string(static_cast<long long>(std::llround(x)));
    }
    return f(x);
}

int main() {
    double a, b, c, d;
    std::cin >> a >> b >> c >> d;
    double det = a * d - b * c;

    std::cout << "行列式 = " << num(det) << std::endl;
    std::cout << (std::abs(det) < EPS ? "不可逆" : "可逆") << std::endl;
    return 0;
}
