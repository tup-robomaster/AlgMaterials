// 4-19 直线拟合
// 拟合 y = a x + b，正规方程是 [n Σx; Σx Σx²] [b; a] = [Σy; Σxy]。
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Fit {
    double a;
    double b;
    double sse;
};

Fit fit(const std::vector<std::pair<double, double>>& pts) {
    int n = static_cast<int>(pts.size());
    double sx = 0.0, sy = 0.0, sxx = 0.0, sxy = 0.0;
    for (const auto& p : pts) {
        sx += p.first;
        sy += p.second;
        sxx += p.first * p.first;
        sxy += p.first * p.second;
    }
    double a = (n * sxy - sx * sy) / (n * sxx - sx * sx);
    double b = (sy - a * sx) / n;
    double sse = 0.0;
    for (const auto& p : pts) {
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
    int n;
    std::cin >> n;
    std::vector<std::pair<double, double>> pts(n);
    for (auto& p : pts) {
        std::cin >> p.first >> p.second;
    }

    Fit r = fit(pts);
    std::cout << "a = " << f(r.a) << std::endl;
    std::cout << "b = " << f(r.b) << std::endl;
    std::cout << "残差平方和 = " << f(r.sse) << std::endl;
    return 0;
}
