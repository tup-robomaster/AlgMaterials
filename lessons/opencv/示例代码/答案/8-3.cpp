#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {

const double kPi = 3.14159265358979323846;

double unwrapAngle(double previous, double current) {
    double value = current;
    while (value - previous > kPi) {
        value -= 2.0 * kPi;
    }
    while (value - previous < -kPi) {
        value += 2.0 * kPi;
    }
    return value;
}

}  // namespace

int main() {
    const std::vector<double> wrapped = {3.00, 3.20, -3.10, -2.90, 3.00};
    double previous = wrapped[0];

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "unwrapped =";
    for (std::size_t k = 0; k < wrapped.size(); ++k) {
        if (k > 0) {
            previous = unwrapAngle(previous, wrapped[k]);
        }
        std::cout << ' ' << previous;
    }
    std::cout << '\n';
    return 0;
}
