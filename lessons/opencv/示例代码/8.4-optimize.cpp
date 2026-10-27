#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace {

constexpr double kDt = 1.0 / 30.0;

struct Config {
    double process_position;
    double process_velocity;
    double measurement;
};

class Tracker {
public:
    Tracker(const Config& config, double first_measurement,
            double initial_rate)
        : config_(config) {
        state_ = (cv::Mat_<double>(2, 1) << first_measurement, initial_rate);
        covariance_ = cv::Mat::eye(2, 2, CV_64F);
        transition_ = (cv::Mat_<double>(2, 2) << 1.0, kDt, 0.0, 1.0);
        observation_ = (cv::Mat_<double>(1, 2) << 1.0, 0.0);
        process_ = (cv::Mat_<double>(2, 2) <<
            config.process_position, 0.0,
            0.0, config.process_velocity);
        measurement_ = (cv::Mat_<double>(1, 1) << config.measurement);
    }

    double update(double measurement) {
        state_ = transition_ * state_;
        covariance_ = transition_ * covariance_ * transition_.t() + process_;

        const cv::Mat innovation_cov =
            observation_ * covariance_ * observation_.t() + measurement_;
        const cv::Mat gain =
            covariance_ * observation_.t() * innovation_cov.inv();
        const cv::Mat residual = (cv::Mat_<double>(1, 1) << measurement) -
                                 observation_ * state_;
        state_ = state_ + gain * residual;
        covariance_ =
            (cv::Mat::eye(2, 2, CV_64F) - gain * observation_) * covariance_;
        return state_.at<double>(0, 0);
    }

    double position() const { return state_.at<double>(0, 0); }
    double rate() const { return state_.at<double>(1, 0); }

private:
    Config config_;
    cv::Mat state_;
    cv::Mat covariance_;
    cv::Mat transition_;
    cv::Mat observation_;
    cv::Mat process_;
    cv::Mat measurement_;
};

cv::Mat makePlot(int width, int height, double x_max, double y_min,
                 double y_max, const std::string& title) {
    cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(252, 252, 250));
    const int left = 95;
    const int right = 45;
    const int top = 70;
    const int bottom = 70;
    const cv::Scalar axis(60, 60, 60);
    const cv::Scalar grid(220, 220, 220);
    const int plot_height = height - top - bottom;
    for (int i = 0; i <= 5; ++i) {
        const int x = left + (width - left - right) * i / 5;
        cv::line(canvas, cv::Point(x, top), cv::Point(x, height - bottom),
                 grid, 1, cv::LINE_AA);
        cv::putText(canvas, cv::format("%.0f", x_max * i / 5.0),
                    cv::Point(x - 10, height - bottom + 24),
                    cv::FONT_HERSHEY_SIMPLEX, 0.45, axis, 1, cv::LINE_AA);
    }
    for (int i = 0; i <= 4; ++i) {
        const double value = y_min + (y_max - y_min) * i / 4.0;
        const int y = height - bottom -
                      cvRound(plot_height * (value - y_min) / (y_max - y_min));
        cv::line(canvas, cv::Point(left, y), cv::Point(width - right, y),
                 grid, 1, cv::LINE_AA);
        cv::putText(canvas, cv::format("%.2f", value),
                    cv::Point(35, y + 5), cv::FONT_HERSHEY_SIMPLEX, 0.45,
                    axis, 1, cv::LINE_AA);
    }
    const int zero_y = height - bottom -
                       cvRound(plot_height * (0.0 - y_min) / (y_max - y_min));
    cv::line(canvas, cv::Point(left, zero_y), cv::Point(width - right, zero_y),
             axis, 1, cv::LINE_AA);
    cv::line(canvas, cv::Point(left, top), cv::Point(left, height - bottom),
             axis, 2, cv::LINE_AA);
    cv::line(canvas, cv::Point(left, height - bottom),
             cv::Point(width - right, height - bottom), axis, 2, cv::LINE_AA);
    cv::putText(canvas, title, cv::Point(left, 38),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(20, 20, 20), 2,
                cv::LINE_AA);
    cv::putText(canvas, "time (s)", cv::Point(width / 2 - 30, height - 18),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, axis, 1, cv::LINE_AA);
    cv::putText(canvas, "error", cv::Point(8, top - 18),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, axis, 1, cv::LINE_AA);
    return canvas;
}

cv::Point toPixel(double x, double y, double x_max, double y_min,
                  double y_max, int width, int height) {
    const int left = 95;
    const int right = 45;
    const int top = 70;
    const int bottom = 70;
    const int px = left + cvRound((width - left - right) * x / x_max);
    const int py = height - bottom -
                   cvRound((height - top - bottom) * (y - y_min) /
                           (y_max - y_min));
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
    const int count = 300;
    const double duration = count * kDt;
    const double y_min = -0.12;
    const double y_max = 0.12;

    std::vector<double> truth(count);
    std::vector<double> measured(count);
    for (int k = 0; k < count; ++k) {
        const double t = k * kDt;
        truth[k] = 0.2 + 0.8 * t + 0.15 * std::sin(0.7 * t);
        measured[k] = truth[k] + 0.04 * std::sin(1.3 * k) +
                      0.03 * std::cos(0.9 * k) + 0.02 * std::sin(0.21 * k);
    }

    const Config baseline_config{1e-3, 1e-2, 1e-4};
    const Config tuned_config{1e-6, 1e-4, 1.6e-3};
    Tracker baseline(baseline_config, measured[0], 0.8);
    Tracker tuned(tuned_config, measured[0], 0.8);

    std::vector<double> baseline_path(count);
    std::vector<double> tuned_path(count);
    std::vector<double> smooth_path(count);
    double smooth = measured[0];
    double raw_sq = 0.0;
    double baseline_sq = 0.0;
    double tuned_sq = 0.0;
    double moving_sq = 0.0;
    double smooth_sq = 0.0;
    double pred_short_sum = 0.0;
    double pred_long_sum = 0.0;
    int pred_short_count = 0;
    int pred_long_count = 0;

    for (int k = 0; k < count; ++k) {
        const double base_position = baseline.update(measured[k]);
        const double tuned_position = tuned.update(measured[k]);
        baseline_path[k] = base_position;
        tuned_path[k] = tuned_position;

        smooth = 0.7 * smooth + 0.3 * tuned_position;
        smooth_path[k] = smooth;
        double window_sum = 0.0;
        int window_count = 0;
        for (int j = std::max(0, k - 4); j <= k; ++j) {
            window_sum += measured[j];
            ++window_count;
        }
        const double moving = window_sum / window_count;

        raw_sq += (measured[k] - truth[k]) * (measured[k] - truth[k]);
        baseline_sq += (base_position - truth[k]) * (base_position - truth[k]);
        tuned_sq += (tuned_position - truth[k]) * (tuned_position - truth[k]);
        moving_sq += (moving - truth[k]) * (moving - truth[k]);
        smooth_sq += (smooth - truth[k]) * (smooth - truth[k]);

        const int short_steps = static_cast<int>(std::lround(0.5 / kDt));
        const int long_steps = static_cast<int>(std::lround(1.0 / kDt));
        if (k + short_steps < count) {
            const double pred = tuned.position() + tuned.rate() * 0.5;
            pred_short_sum += std::abs(pred - truth[k + short_steps]);
            ++pred_short_count;
        }
        if (k + long_steps < count) {
            const double pred = tuned.position() + tuned.rate() * 1.0;
            pred_long_sum += std::abs(pred - truth[k + long_steps]);
            ++pred_long_count;
        }
    }

    cv::Mat canvas = makePlot(1200, 640, duration, y_min, y_max,
                              "error relative to the true position");
    std::vector<cv::Point> baseline_points;
    std::vector<cv::Point> tuned_points;
    std::vector<cv::Point> smooth_points;
    for (int k = 0; k < count; ++k) {
        const double t = k * kDt;
        baseline_points.push_back(toPixel(
            t, baseline_path[k] - truth[k], duration, y_min, y_max, 1200, 640));
        tuned_points.push_back(toPixel(
            t, tuned_path[k] - truth[k], duration, y_min, y_max, 1200, 640));
        smooth_points.push_back(toPixel(
            t, smooth_path[k] - truth[k], duration, y_min, y_max, 1200, 640));
        cv::circle(canvas,
                   toPixel(t, measured[k] - truth[k], duration, y_min, y_max,
                           1200, 640),
                   2, cv::Scalar(175, 175, 175), cv::FILLED, cv::LINE_AA);
    }
    cv::polylines(canvas, baseline_points, false, cv::Scalar(30, 140, 230), 2,
                  cv::LINE_AA);
    cv::polylines(canvas, smooth_points, false, cv::Scalar(60, 150, 50), 2,
                  cv::LINE_AA);
    cv::polylines(canvas, tuned_points, false, cv::Scalar(40, 60, 220), 3,
                  cv::LINE_AA);
    legend(canvas, 660, 105, cv::Scalar(175, 175, 175), "measurement");
    legend(canvas, 660, 140, cv::Scalar(30, 140, 230), "baseline");
    legend(canvas, 820, 105, cv::Scalar(60, 150, 50), "smoothed");
    legend(canvas, 820, 140, cv::Scalar(40, 60, 220), "tuned");

    const bool saved = cv::imwrite("../素材/out_kalman_compare.png", canvas);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "frames = " << count << '\n';
    std::cout << "raw_rmse = " << std::sqrt(raw_sq / count) << '\n';
    std::cout << "baseline_rmse = " << std::sqrt(baseline_sq / count) << '\n';
    std::cout << "tuned_rmse = " << std::sqrt(tuned_sq / count) << '\n';
    std::cout << "moving_average_rmse = " << std::sqrt(moving_sq / count)
              << '\n';
    std::cout << "smoothed_rmse = " << std::sqrt(smooth_sq / count) << '\n';
    std::cout << "prediction_0.5s = " << pred_short_sum / pred_short_count
              << '\n';
    std::cout << "prediction_1.0s = " << pred_long_sum / pred_long_count
              << '\n';
    std::cout << "saved out_kalman_compare.png " << canvas.cols << "x"
              << canvas.rows << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
