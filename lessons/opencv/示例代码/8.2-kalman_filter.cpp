#include <cmath>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>

int main() {
    const int count = 150;
    const float dt = 0.05f;
    const float q = 0.05f;
    const float r = 0.02f;

    cv::KalmanFilter kf(2, 1, 0, CV_32F);
    kf.transitionMatrix = (cv::Mat_<float>(2, 2) <<
        1.0f, dt,
        0.0f, 1.0f);
    kf.measurementMatrix = (cv::Mat_<float>(1, 2) << 1.0f, 0.0f);
    kf.processNoiseCov = (cv::Mat_<float>(2, 2) <<
        q * dt * dt * dt * dt / 4.0f, q * dt * dt * dt / 2.0f,
        q * dt * dt * dt / 2.0f, q * dt * dt);
    kf.measurementNoiseCov = (cv::Mat_<float>(1, 1) << r);
    kf.errorCovPost = cv::Mat::eye(2, 2, CV_32F) * 1.0f;
    kf.statePost = (cv::Mat_<float>(2, 1) << 0.2f, 0.0f);

    double raw_sq = 0.0;
    double filtered_sq = 0.0;

    std::cout << std::fixed << std::setprecision(4);
    for (int k = 0; k < count; ++k) {
        const float t = k * dt;
        const float truth = 0.2f + 1.0f * t;
        const float noise = 0.03f * std::sin(1.3f * k) +
                            0.02f * std::cos(0.7f * k);
        const float measured = truth + noise;

        kf.predict();
        cv::Mat measurement = (cv::Mat_<float>(1, 1) << measured);
        kf.correct(measurement);

        const float estimate = kf.statePost.at<float>(0, 0);
        const double e_raw = measured - truth;
        const double e_filtered = estimate - truth;
        raw_sq += e_raw * e_raw;
        filtered_sq += e_filtered * e_filtered;

        if (k % 30 == 0 || k == count - 1) {
            std::cout << "step=" << std::setw(3) << k
                      << " angle=" << std::setw(8) << estimate
                      << " omega=" << std::setw(8)
                      << kf.statePost.at<float>(1, 0)
                      << " p00=" << std::setw(8)
                      << kf.errorCovPost.at<float>(0, 0)
                      << " p11=" << std::setw(8)
                      << kf.errorCovPost.at<float>(1, 1) << '\n';
        }
    }

    std::cout << std::setprecision(6);
    std::cout << "raw_rmse = " << std::sqrt(raw_sq / count) << '\n';
    std::cout << "filtered_rmse = " << std::sqrt(filtered_sq / count) << '\n';
    std::cout << std::setprecision(4);
    std::cout << "final_angle = " << kf.statePost.at<float>(0, 0) << '\n';
    std::cout << "final_omega = " << kf.statePost.at<float>(1, 0) << '\n';
    std::cout << "transitionMatrix =\n" << kf.transitionMatrix << '\n';
    std::cout << "measurementMatrix =\n" << kf.measurementMatrix << '\n';
    return 0;
}
