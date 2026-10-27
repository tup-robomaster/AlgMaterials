#include <cmath>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>

int main() {
    const int count = 200;
    const double dt = 0.02;
    const double q_position = 1e-6;
    const double q_velocity = 1e-2;
    const double r = 0.01;

    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, dt, 0.0, 1.0);
    const cv::Mat Q = (cv::Mat_<double>(2, 2) << q_position, 0.0,
                       0.0, q_velocity);
    cv::Mat state = (cv::Mat_<double>(2, 1) << std::sqrt(1.52), 0.25);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);
    double ekf_sq = 0.0;
    for (int k = 0; k < count; ++k) {
        const double truth = 1.2 + 0.25 * (k * dt);
        const double z = truth * truth + 0.12 * std::sin(1.1 * k) +
                         0.08 * std::cos(0.4 * k);
        if (k == 0) {
            state.at<double>(0, 0) = std::sqrt(z);
        }

        state = F * state;
        P = F * P * F.t() + Q;
        const double x = state.at<double>(0, 0);
        const double predicted_z = x * x;
        const cv::Mat H = (cv::Mat_<double>(1, 2) << 2.0 * x, 0.0);
        const cv::Mat S = H * P * H.t() + r;
        const cv::Mat K = P * H.t() * S.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << z - predicted_z);
        state = state + K * residual;
        P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;

        const double error = state.at<double>(0, 0) - truth;
        ekf_sq += error * error;
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "ekf_rmse = " << std::sqrt(ekf_sq / count) << '\n';
    std::cout << "final_x = " << state.at<double>(0, 0) << '\n';
    return 0;
}
