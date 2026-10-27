#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace {

constexpr double kDt = 0.02;

cv::Mat makePlot(int width, int height, double x_max, double y_max,
                 const std::string& title) {
    cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(252, 252, 250));
    const int left = 95;
    const int right = 45;
    const int top = 70;
    const int bottom = 70;
    const cv::Scalar axis(60, 60, 60);
    const cv::Scalar grid(220, 220, 220);
    for (int i = 0; i <= 4; ++i) {
        const int x = left + (width - left - right) * i / 4;
        cv::line(canvas, cv::Point(x, top), cv::Point(x, height - bottom),
                 grid, 1, cv::LINE_AA);
        cv::putText(canvas, cv::format("%.0f", x_max * i / 4.0),
                    cv::Point(x - 10, height - bottom + 24),
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
    cv::putText(canvas, "value", cv::Point(8, top - 18),
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

double square(double value) { return value * value; }

std::vector<double> runEkf(const std::vector<double>& measured_z) {
    const double q_position = 1e-6;
    const double q_velocity = 1e-4;
    const double r = 0.01;

    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, kDt, 0.0, 1.0);
    const cv::Mat Q = (cv::Mat_<double>(2, 2) << q_position, 0.0,
                       0.0, q_velocity);
    cv::Mat state =
        (cv::Mat_<double>(2, 1) << std::sqrt(measured_z[0]), 0.25);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);
    std::vector<double> path;
    path.reserve(measured_z.size());
    for (double z : measured_z) {
        state = F * state;
        P = F * P * F.t() + Q;

        const double predicted_z = square(state.at<double>(0, 0));
        const cv::Mat H =
            (cv::Mat_<double>(1, 2) << 2.0 * state.at<double>(0, 0), 0.0);
        const cv::Mat S = H * P * H.t() + r;
        const cv::Mat K = P * H.t() * S.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << z - predicted_z);
        state = state + K * residual;
        P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;
        path.push_back(state.at<double>(0, 0));
    }
    return path;
}

std::vector<double> runLinear(const std::vector<double>& measured_z) {
    const double q_position = 1e-6;
    const double q_velocity = 1e-4;
    const double r = 0.01;

    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, kDt, 0.0, 1.0);
    const cv::Mat Q = (cv::Mat_<double>(2, 2) << q_position, 0.0,
                       0.0, q_velocity);
    const cv::Mat H = (cv::Mat_<double>(1, 2) << 1.0, 0.0);
    cv::Mat state = (cv::Mat_<double>(2, 1) << measured_z[0], 0.25);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);
    std::vector<double> path;
    path.reserve(measured_z.size());
    for (double z : measured_z) {
        state = F * state;
        P = F * P * F.t() + Q;

        const cv::Mat S = H * P * H.t() + r;
        const cv::Mat K = P * H.t() * S.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << z) - H * state;
        state = state + K * residual;
        P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;
        path.push_back(state.at<double>(0, 0));
    }
    return path;
}

double rmse(const std::vector<double>& path, const std::vector<double>& truth) {
    double sum = 0.0;
    for (std::size_t k = 0; k < path.size(); ++k) {
        sum += square(path[k] - truth[k]);
    }
    return std::sqrt(sum / path.size());
}

}  // namespace

int main() {
    const int count = 200;
    const double duration = count * kDt;

    std::vector<double> truth(count);
    std::vector<double> measured_z(count);
    for (int k = 0; k < count; ++k) {
        truth[k] = 1.2 + 0.25 * (k * kDt);
        measured_z[k] = square(truth[k]) + 0.12 * std::sin(1.1 * k) +
                        0.08 * std::cos(0.4 * k);
    }

    const std::vector<double> ekf_path = runEkf(measured_z);
    const std::vector<double> linear_path = runLinear(measured_z);

    const double ekf_error = rmse(ekf_path, truth);
    const double linear_error = rmse(linear_path, truth);

    const double final_x = ekf_path.back();
    const double eps = 1e-6;
    const double numeric_jacobian =
        (square(final_x + eps) - square(final_x - eps)) / (2.0 * eps);

    cv::Mat canvas = makePlot(1200, 640, duration, 5.0,
                              "nonlinear measurement z = x squared");
    std::vector<cv::Point> truth_points;
    std::vector<cv::Point> ekf_points;
    std::vector<cv::Point> linear_points;
    for (int k = 0; k < count; ++k) {
        const double t = k * kDt;
        truth_points.push_back(toPixel(t, truth[k], duration, 5.0, 1200, 640));
        ekf_points.push_back(
            toPixel(t, ekf_path[k], duration, 5.0, 1200, 640));
        linear_points.push_back(
            toPixel(t, linear_path[k], duration, 5.0, 1200, 640));
        cv::circle(canvas, toPixel(t, measured_z[k], duration, 5.0, 1200, 640),
                   2, cv::Scalar(165, 165, 165), cv::FILLED, cv::LINE_AA);
    }
    cv::polylines(canvas, truth_points, false, cv::Scalar(200, 90, 40), 6,
                  cv::LINE_AA);
    cv::polylines(canvas, linear_points, false, cv::Scalar(30, 140, 230), 2,
                  cv::LINE_AA);
    cv::polylines(canvas, ekf_points, false, cv::Scalar(40, 60, 220), 2,
                  cv::LINE_AA);
    legend(canvas, 690, 105, cv::Scalar(200, 90, 40), "truth x");
    legend(canvas, 850, 105, cv::Scalar(165, 165, 165), "measured z");
    legend(canvas, 690, 140, cv::Scalar(30, 140, 230), "linear filter");
    legend(canvas, 850, 140, cv::Scalar(40, 60, 220), "ekf");

    const bool saved = cv::imwrite("../素材/out_ekf.png", canvas);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "steps = " << count << '\n';
    std::cout << "ekf_rmse = " << ekf_error << '\n';
    std::cout << "linear_rmse = " << linear_error << '\n';
    std::cout << "final_x = " << final_x << '\n';
    std::cout << "jacobian_at_final = " << 2.0 * final_x << '\n';
    std::cout << "numeric_jacobian = " << numeric_jacobian << '\n';
    std::cout << "saved out_ekf.png " << canvas.cols << "x" << canvas.rows
              << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
