#include <cmath>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>

int main() {
    const int count = 300;
    const double dt = 1.0 / 30.0;
    const double q_position = 1e-6;
    const double q_velocity = 1e-4;
    const double r = 1.6e-3;

    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, dt, 0.0, 1.0);
    const cv::Mat H = (cv::Mat_<double>(1, 2) << 1.0, 0.0);
    const cv::Mat Q = (cv::Mat_<double>(2, 2) << q_position, 0.0,
                       0.0, q_velocity);
    cv::Mat state = (cv::Mat_<double>(2, 1) << 0.0, 0.8);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);

    double smoothed = 0.0;
    double smooth_sq = 0.0;
    for (int k = 0; k < count; ++k) {
        const double t = k * dt;
        const double truth = 0.2 + 0.8 * t + 0.15 * std::sin(0.7 * t);
        const double measured = truth + 0.04 * std::sin(1.3 * k) +
                                0.03 * std::cos(0.9 * k) +
                                0.02 * std::sin(0.21 * k);
        if (k == 0) {
            state.at<double>(0, 0) = measured;
            smoothed = measured;
        }

        state = F * state;
        P = F * P * F.t() + Q;
        const cv::Mat S = H * P * H.t() + r;
        const cv::Mat K = P * H.t() * S.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << measured) -
                                 H * state;
        state = state + K * residual;
        P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;

        const double tuned = state.at<double>(0, 0);
        smoothed = 0.9 * smoothed + 0.1 * tuned;
        const double error = smoothed - truth;
        smooth_sq += error * error;
    }
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "smoothed_rmse = " << std::sqrt(smooth_sq / count) << '\n';
    return 0;
}
