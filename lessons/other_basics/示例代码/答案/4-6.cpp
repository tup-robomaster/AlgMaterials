// 4-6 三点求三角形面积
// 面积 = |(P2-P1) × (P3-P1)| / 2，二维叉积就是那两项之差。
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    double x1, y1, x2, y2, x3, y3;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    double area = std::abs((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)) / 2.0;
    std::cout << "面积 = " << std::fixed << std::setprecision(3) << area
              << std::endl;
    return 0;
}
