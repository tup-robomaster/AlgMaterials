#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

cv::Mat makePlot(int width, int height, double x_max, double y_max,
                 const std::string& title) {
    cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(252, 252, 250));
    const int left = 95;
    const int right = 45;
    const int top = 70;
    const int bottom = 70;
    const cv::Scalar axis(60, 60, 60);
    const cv::Scalar grid(220, 220, 220);
    for (int i = 0; i <= 10; ++i) {
        const int x = left + (width - left - right) * i / 10;
        cv::line(canvas, cv::Point(x, top), cv::Point(x, height - bottom),
                 grid, 1, cv::LINE_AA);
        cv::putText(canvas, cv::format("%.1f", x_max * i / 10.0),
                    cv::Point(x - 18, height - bottom + 24),
                    cv::FONT_HERSHEY_SIMPLEX, 0.45, axis, 1, cv::LINE_AA);
    }
    for (int i = 0; i <= 5; ++i) {
        const int y = height - bottom - (height - top - bottom) * i / 5;
        cv::line(canvas, cv::Point(left, y), cv::Point(width - right, y),
                 grid, 1, cv::LINE_AA);
        cv::putText(canvas, cv::format("%.1f", y_max * i / 5.0),
                    cv::Point(35, y + 5), cv::FONT_HERSHEY_SIMPLEX, 0.45,
                    axis, 1, cv::LINE_AA);
    }
    cv::line(canvas, cv::Point(left, top), cv::Point(left, height - bottom),
             axis, 2, cv::LINE_AA);
    cv::line(canvas, cv::Point(left, height - bottom),
             cv::Point(width - right, height - bottom), axis, 2, cv::LINE_AA);
    cv::putText(canvas, title, cv::Point(left, 38),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(20, 20, 20), 2,
                cv::LINE_AA);
    cv::putText(canvas, "time (s)", cv::Point(width / 2 - 30, height - 18),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, axis, 1, cv::LINE_AA);
    cv::putText(canvas, "position", cv::Point(8, top - 18),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, axis, 1, cv::LINE_AA);
    return canvas;
}

cv::Point toPixel(double x, double y, double x_max, double y_max,
                  int width, int height) {
    const int left = 95;
    const int right = 45;
    const int top = 70;
    const int bottom = 70;
    const int px = left + cvRound((width - left - right) * x / x_max);
    const int py = height - bottom -
                   cvRound((height - top - bottom) * y / y_max);
    return cv::Point(px, py);
}

void legend(cv::Mat& canvas, int x, int y, const cv::Scalar& color,
            const std::string& text) {
    cv::line(canvas, cv::Point(x, y), cv::Point(x + 34, y), color, 3,
             cv::LINE_AA);
    cv::putText(canvas, text, cv::Point(x + 44, y + 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(40, 40, 40), 1,
                cv::LINE_AA);
}

}  // namespace

int main() {
    const int count = 100;
    const double dt = 0.1;
    const double x0 = 0.5;
    const double v0 = 1.2;
    const double q = 0.02;
    const double r = 0.04;

    std::vector<double> truth(count);
    std::vector<double> measured(count);
    std::vector<double> filtered(count);

    cv::Mat state = (cv::Mat_<double>(2, 1) << x0 + 0.2, 0.0);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);
    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, dt, 0.0, 1.0);
    const cv::Mat H = (cv::Mat_<double>(1, 2) << 1.0, 0.0);
    const cv::Mat Q = (cv::Mat_<double>(2, 2) <<
        q * dt * dt * dt * dt / 4.0, q * dt * dt * dt / 2.0,
        q * dt * dt * dt / 2.0, q * dt * dt);

    for (int k = 0; k < count; ++k) {
        const double t = k * dt;
        truth[k] = x0 + v0 * t;
        measured[k] = truth[k] + 0.15 * std::sin(1.7 * k) +
                      0.10 * std::cos(0.9 * k) + 0.05 * std::sin(0.3 * k);

        state = F * state;
        P = F * P * F.t() + Q;

        const cv::Mat S = H * P * H.t() + r;
        const cv::Mat K = P * H.t() * S.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << measured[k]) -
                                 H * state;
        state = state + K * residual;
        P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;
        filtered[k] = state.at<double>(0, 0);
    }

    double raw_sq = 0.0;
    double filtered_sq = 0.0;
    for (int k = 0; k < count; ++k) {
        const double e_raw = measured[k] - truth[k];
        const double e_filtered = filtered[k] - truth[k];
        raw_sq += e_raw * e_raw;
        filtered_sq += e_filtered * e_filtered;
    }
    const double raw_rmse = std::sqrt(raw_sq / count);
    const double filtered_rmse = std::sqrt(filtered_sq / count);

    cv::Mat canvas = makePlot(1200, 640, count * dt, 14.0,
                              "one-dimensional constant velocity model");
    std::vector<cv::Point> true_points;
    std::vector<cv::Point> filtered_points;
    for (int k = 0; k < count; ++k) {
        true_points.push_back(toPixel(k * dt, truth[k], count * dt, 14.0,
                                      1200, 640));
        filtered_points.push_back(toPixel(k * dt, filtered[k], count * dt,
                                          14.0, 1200, 640));
        cv::circle(canvas, toPixel(k * dt, measured[k], count * dt, 14.0,
                                   1200, 640), 3, cv::Scalar(150, 150, 150),
                   cv::FILLED, cv::LINE_AA);
    }
    cv::polylines(canvas, true_points, false, cv::Scalar(200, 90, 40), 2,
                  cv::LINE_AA);
    cv::polylines(canvas, filtered_points, false, cv::Scalar(40, 60, 220), 3,
                  cv::LINE_AA);
    legend(canvas, 720, 105, cv::Scalar(200, 90, 40), "truth");
    legend(canvas, 880, 105, cv::Scalar(150, 150, 150), "measurement");
    legend(canvas, 720, 140, cv::Scalar(40, 60, 220), "filtered");

    const bool saved = cv::imwrite("../素材/synth_kalman_1d.png", canvas);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "steps = " << count << '\n';
    std::cout << "raw_rmse = " << raw_rmse << '\n';
    std::cout << "filtered_rmse = " << filtered_rmse << '\n';
    std::cout << "final_position = " << state.at<double>(0, 0) << '\n';
    std::cout << "final_velocity = " << state.at<double>(1, 0) << '\n';
    std::cout << "saved synth_kalman_1d.png " << canvas.cols << "x"
              << canvas.rows << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
