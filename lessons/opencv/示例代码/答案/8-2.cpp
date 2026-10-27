#include <cmath>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>

int main() {
    const int count = 150;
    const float dt = 0.05f;
    const float q = 0.05f;
    const float r = 0.08f;

    cv::KalmanFilter kf(2, 1, 0, CV_32F);
    kf.transitionMatrix = (cv::Mat_<float>(2, 2) <<
        1.0f, dt,
        0.0f, 1.0f);
    kf.measurementMatrix = (cv::Mat_<float>(1, 2) << 1.0f, 0.0f);
    kf.processNoiseCov = (cv::Mat_<float>(2, 2) <<
        q * dt * dt * dt * dt / 4.0f, q * dt * dt * dt / 2.0f,
        q * dt * dt * dt / 2.0f, q * dt * dt);
    kf.measurementNoiseCov = (cv::Mat_<float>(1, 1) << r);
    kf.errorCovPost = cv::Mat::eye(2, 2, CV_32F);
    kf.statePost = (cv::Mat_<float>(2, 1) << 0.2f, 0.0f);

    double raw_sq = 0.0;
    double filtered_sq = 0.0;
    for (int k = 0; k < count; ++k) {
        const float t = k * dt;
        const float truth = 0.2f + 1.0f * t;
        const float measured = truth + 0.03f * std::sin(1.3f * k) +
                               0.02f * std::cos(0.7f * k);
        kf.predict();
        kf.correct((cv::Mat_<float>(1, 1) << measured));
        const float estimate = kf.statePost.at<float>(0, 0);
        raw_sq += (measured - truth) * (measured - truth);
        filtered_sq += (estimate - truth) * (estimate - truth);
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "raw_rmse = " << std::sqrt(raw_sq / count) << '\n';
    std::cout << "filtered_rmse = " << std::sqrt(filtered_sq / count) << '\n';
    std::cout << std::setprecision(4);
    std::cout << "final_omega = " << kf.statePost.at<float>(1, 0) << '\n';
    return 0;
}
