#include <iomanip>
#include <iostream>

int main() {
    const double k1 = 0.2;
    const double k2 = 0.05;
    std::cout << std::fixed << std::setprecision(6);
    for (int i = 1; i <= 5; ++i) {
        const double r = 0.1 * i;
        const double r2 = r * r;
        const double factor = 1.0 + k1 * r2 + k2 * r2 * r2;
        std::cout << "r = " << r << "  factor = " << factor
                  << "  r' = " << r * factor << '\n';
    }
    return 0;
}
