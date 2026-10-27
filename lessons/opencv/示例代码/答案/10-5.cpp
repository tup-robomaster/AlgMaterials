#include <iomanip>
#include <iostream>

int main() {
    const double dt = 0.5;
    const double x0 = 10.0;
    const double x1 = 2.0;
    const double p00 = 4.0;
    const double p01 = 0.0;
    const double p10 = 0.0;
    const double p11 = 1.0;
    const double q0 = 0.1;
    const double q1 = 0.02;
    const double measurement = 11.5;
    const double r = 0.5;

    const double predicted_x0 = x0 + dt * x1;
    const double predicted_x1 = x1;
    const double predicted_p00 = p00 + dt * (p01 + p10) +
                                 dt * dt * p11 + q0;
    const double predicted_p01 = p01 + dt * p11;
    const double predicted_p10 = p10 + dt * p11;
    const double predicted_p11 = p11 + q1;

    const double innovation = measurement - predicted_x0;
    const double s = predicted_p00 + r;
    const double k0 = predicted_p00 / s;
    const double k1 = predicted_p10 / s;
    const double updated_x0 = predicted_x0 + k0 * innovation;
    const double updated_x1 = predicted_x1 + k1 * innovation;
    const double updated_p00 = predicted_p00 - k0 * predicted_p00;
    const double updated_p01 = predicted_p01 - k0 * predicted_p01;
    const double updated_p10 = predicted_p10 - k1 * predicted_p00;
    const double updated_p11 = predicted_p11 - k1 * predicted_p01;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "predict:\n";
    std::cout << "  x = (" << predicted_x0 << ", " << predicted_x1 << ")\n";
    std::cout << "  P = [[" << predicted_p00 << ", " << predicted_p01
              << "], [" << predicted_p10 << ", " << predicted_p11 << "]]\n";
    std::cout << "update:\n";
    std::cout << "  innovation = " << innovation << '\n';
    std::cout << "  S = " << s << '\n';
    std::cout << "  K = (" << k0 << ", " << k1 << ")\n";
    std::cout << "  x = (" << updated_x0 << ", " << updated_x1 << ")\n";
    std::cout << "  P = [[" << updated_p00 << ", " << updated_p01 << "], ["
              << updated_p10 << ", " << updated_p11 << "]]\n";
    return 0;
}
