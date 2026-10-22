// 4.7：直线拟合与最小二乘。
//
// 拟合 y = a x + b，正规方程是 [n Σx; Σx Σx²] [b; a] = [Σy; Σxy]。
// 解出 a、b 后，残差平方和是每个点的误差平方之和。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Point = std::pair<double, double>;

struct Fit {
    double a;
    double b;
    double sse;
};

Fit fit(const std::vector<Point>& points, bool through_origin = false) {
    int n = static_cast<int>(points.size());
    double sx = 0.0, sy = 0.0, sxx = 0.0, sxy = 0.0;
    for (const Point& p : points) {
        sx += p.first;
        sy += p.second;
        sxx += p.first * p.first;
        sxy += p.first * p.second;
    }
    double a, b;
    if (through_origin) {
        a = sxy / sxx;
        b = 0.0;
    } else {
        double det = n * sxx - sx * sx;
        a = (n * sxy - sx * sy) / det;
        b = (sy - a * sx) / n;
    }
    double sse = 0.0;
    for (const Point& p : points) {
        double e = p.second - (a * p.first + b);
        sse += e * e;
    }
    return {a, b, sse};
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
    std::vector<Point> points = {{1.0, 2.0}, {2.0, 3.0}, {3.0, 5.0}, {4.0, 7.0}};
    Fit result = fit(points);

    std::cout << "a = " << f(result.a) << ", b = " << f(result.b) << std::endl;
    std::cout << "残差平方和 = " << f(result.sse) << std::endl;
    std::cout << "x = 5 时的预测值 = " << f(result.a * 5.0 + result.b)
              << std::endl;
    return 0;
}
