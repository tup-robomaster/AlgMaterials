#include <cmath>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>

int main() {
    const int count = 100;
    const double dt = 0.1;
    const double q = 0.02;
    const double r = 0.04;

    cv::Mat state = (cv::Mat_<double>(2, 1) << 0.7, 0.0);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);
    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, dt, 0.0, 1.0);
    const cv::Mat H = (cv::Mat_<double>(1, 2) << 1.0, 0.0);
    const cv::Mat Q = (cv::Mat_<double>(2, 2) <<
        q * dt * dt * dt * dt / 4.0, q * dt * dt * dt / 2.0,
        q * dt * dt * dt / 2.0, q * dt * dt);

    double raw_sq = 0.0;
    double filtered_sq = 0.0;
    for (int k = 0; k < count; ++k) {
        const double truth = 0.5 + 1.2 * (k * dt);
        const double measured = truth + 0.075 * std::sin(1.7 * k) +
                                0.05 * std::cos(0.9 * k) +
                                0.025 * std::sin(0.3 * k);
        state = F * state;
        P = F * P * F.t() + Q;
        const cv::Mat S = H * P * H.t() + r;
        const cv::Mat K = P * H.t() * S.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << measured) -
                                 H * state;
        state = state + K * residual;
        P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;

        raw_sq += (measured - truth) * (measured - truth);
        const double error = state.at<double>(0, 0) - truth;
        filtered_sq += error * error;
    }

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "raw_rmse = " << std::sqrt(raw_sq / count) << '\n';
    std::cout << "filtered_rmse = " << std::sqrt(filtered_sq / count) << '\n';
    std::cout << "final_velocity = " << state.at<double>(1, 0) << '\n';
    return 0;
}
