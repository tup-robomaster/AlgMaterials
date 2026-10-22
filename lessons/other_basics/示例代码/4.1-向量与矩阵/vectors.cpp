// 4.1：向量、矩阵与乘法。
//
// 向量用 vector<double> 表示，矩阵用 vector<vector<double>> 表示，
// 一行是一个内层 vector。点积、矩阵乘向量、矩阵乘矩阵都从这里展开。

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Vec = std::vector<double>;
using Mat = std::vector<Vec>;

double dot(const Vec& u, const Vec& v) {
    double s = 0.0;
    for (std::size_t i = 0; i < u.size(); i++) {
        s += u[i] * v[i];
    }
    return s;
}

double length(const Vec& u) {
    return std::sqrt(dot(u, u));
}

Vec mat_vec(const Mat& M, const Vec& x) {
    Vec y;
    for (const Vec& row : M) {
        y.push_back(dot(row, x));
    }
    return y;
}

Mat mat_mul(const Mat& X, const Mat& Y) {
    Mat Z(X.size(), Vec(Y[0].size(), 0.0));
    for (std::size_t i = 0; i < X.size(); i++) {
        for (std::size_t j = 0; j < Y[0].size(); j++) {
            for (std::size_t k = 0; k < Y.size(); k++) {
                Z[i][j] += X[i][k] * Y[k][j];
            }
        }
    }
    return Z;
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

std::string show(const Vec& v) {
    std::string s = "[";
    for (std::size_t i = 0; i < v.size(); i++) {
        if (i > 0) {
            s += ", ";
        }
        s += f(v[i]);
    }
    return s + "]";
}

std::string show(const Mat& M) {
    std::string s = "[";
    for (std::size_t i = 0; i < M.size(); i++) {
        if (i > 0) {
            s += ", ";
        }
        s += show(M[i]);
    }
    return s + "]";
}

int main() {
    Vec a = {1.0, 2.0};
    Vec b = {3.0, 4.0};
    Mat A = {{1.0, 2.0}, {3.0, 4.0}};
    Mat B = {{2.0, 1.0}, {0.0, 3.0}};

    std::cout << "点积 = " << f(dot(a, b)) << std::endl;
    std::cout << "a 的长度 = " << f(length(a)) << std::endl;
    std::cout << "A b = " << show(mat_vec(A, b)) << std::endl;
    std::cout << "A B = " << show(mat_mul(A, B)) << std::endl;
    std::cout << "B A = " << show(mat_mul(B, A)) << std::endl;
    return 0;
}
