// 4-1 向量加法与数乘
// 读入维数、两个向量和一个标量，输出 a+b 与 k*a。
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
    int n;
    std::cin >> n;
    std::vector<double> a(n), b(n);
    for (double& x : a) {
        std::cin >> x;
    }
    for (double& x : b) {
        std::cin >> x;
    }
    double k;
    std::cin >> k;

    std::vector<double> sum(n), scaled(n);
    for (int i = 0; i < n; i++) {
        sum[i] = a[i] + b[i];
        scaled[i] = k * a[i];
    }
    std::cout << show(sum) << std::endl;
    std::cout << show(scaled) << std::endl;
    return 0;
}
