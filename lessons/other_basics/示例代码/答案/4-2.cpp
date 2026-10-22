// 4-2 点积、长度与夹角
// 夹角余弦 = 点积 / (两个长度之积)。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
    std::vector<double> a(n), b(n);
    for (double& x : a) {
        std::cin >> x;
    }
    for (double& x : b) {
        std::cin >> x;
    }

    double dot = 0.0;
    double aa = 0.0;
    double bb = 0.0;
    for (int i = 0; i < n; i++) {
        dot += a[i] * b[i];
        aa += a[i] * a[i];
        bb += b[i] * b[i];
    }
    double la = std::sqrt(aa);
    double lb = std::sqrt(bb);
    double cos = (la != 0.0 && lb != 0.0) ? dot / (la * lb) : 0.0;

    std::cout << "点积 = " << f(dot) << std::endl;
    std::cout << "|a| = " << f(la) << std::endl;
    std::cout << "|b| = " << f(lb) << std::endl;
    std::cout << "cos = " << f(cos) << std::endl;
    return 0;
}
