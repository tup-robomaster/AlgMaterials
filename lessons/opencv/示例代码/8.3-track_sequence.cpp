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

const double kPi = 3.14159265358979323846;

double wrapAngle(double angle) {
    while (angle > kPi) {
        angle -= 2.0 * kPi;
    }
    while (angle < -kPi) {
        angle += 2.0 * kPi;
    }
    return angle;
}

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

cv::Point toPixel(double x, double y, double x_max, double y_max,
                  int width, int height) {
    const int left = 95;
    const int right = 45;
    const int top = 70;
    const int bottom = 70;
    return cv::Point(
        left + cvRound((width - left - right) * x / x_max),
        height - bottom - cvRound((height - top - bottom) * y / y_max));
}

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
                cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(20, 20, 20), 2,
                cv::LINE_AA);
    cv::putText(canvas, "time (s)", cv::Point(width / 2 - 30, height - 18),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, axis, 1, cv::LINE_AA);
    cv::putText(canvas, "angle (rad)", cv::Point(8, top - 18),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, axis, 1, cv::LINE_AA);
    return canvas;
}

void legend(cv::Mat& canvas, int x, int y, const cv::Scalar& color,
            const std::string& text) {
    cv::line(canvas, cv::Point(x, y), cv::Point(x + 34, y), color, 3,
             cv::LINE_AA);
    cv::putText(canvas, text, cv::Point(x + 44, y + 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(40, 40, 40), 1,
                cv::LINE_AA);
}

}  // namespace

int main() {
    const int count = 120;
    const double dt = 1.0 / 30.0;
    std::vector<double> truth(count);
    std::vector<double> raw(count);
    std::vector<double> unwrapped(count, 0.0);
    std::vector<bool> available(count, false);
    for (int k = 0; k < count; ++k) {
        truth[k] = 0.4 + 0.030 * k;
        const double noise = 0.035 * std::sin(0.6 * k) +
                             0.025 * std::cos(1.1 * k) +
                             0.015 * std::sin(0.17 * k);
        raw[k] = wrapAngle(truth[k] + noise);
        available[k] = !(k >= 70 && k <= 79);
    }
    double previous = raw[0];
    unwrapped[0] = raw[0];
    for (int k = 1; k < count; ++k) {
        if (!available[k]) {
            continue;
        }
        unwrapped[k] = unwrapAngle(previous, raw[k]);
        previous = unwrapped[k];
    }

    std::vector<double> averaged(count, raw[0]);
    for (int k = 0; k < count; ++k) {
        double sum = 0.0;
        int used = 0;
        for (int j = std::max(0, k - 7); j <= k; ++j) {
            if (available[j]) {
                sum += unwrapped[j];
                ++used;
            }
        }
        if (used > 0) {
            averaged[k] = sum / used;
        } else {
            averaged[k] = averaged[k - 1];
        }
    }

    cv::Mat state = (cv::Mat_<double>(2, 1) << unwrapped[0], 1.0);
    cv::Mat P = cv::Mat::eye(2, 2, CV_64F);
    const cv::Mat F = (cv::Mat_<double>(2, 2) << 1.0, dt, 0.0, 1.0);
    const cv::Mat H = (cv::Mat_<double>(1, 2) << 1.0, 0.0);
    const double q = 0.02;
    const double r = 0.0036;
    const cv::Mat Q = (cv::Mat_<double>(2, 2) <<
        q * dt * dt * dt * dt / 4.0, q * dt * dt * dt / 2.0,
        q * dt * dt * dt / 2.0, q * dt * dt);

    std::vector<double> filtered(count, 0.0);
    std::vector<double> smoothed(count, 0.0);
    int missing_run = 0;
    int jump_run = 0;
    int rate_run = 0;
    bool lost = false;
    int resets = 0;
    int lost_resets = 0;
    int jump_resets = 0;
    int rate_resets = 0;
    int dropouts = 0;
    double smoothed_value = unwrapped[0];
    double prediction_error = 0.0;
    int prediction_count = 0;

    for (int k = 0; k < count; ++k) {
        state = F * state;
        P = F * P * F.t() + Q;
        if (available[k]) {
            if (lost) {
                state = (cv::Mat_<double>(2, 1) << unwrapped[k],
                         state.at<double>(1, 0));
                P = cv::Mat::eye(2, 2, CV_64F) * 0.5;
                lost = false;
                ++resets;
                ++lost_resets;
            } else {
                const double innovation =
                    unwrapped[k] - state.at<double>(0, 0);
                if (std::fabs(innovation) > kPi / 4.0) {
                    ++jump_run;
                } else {
                    jump_run = 0;
                }
                if (jump_run >= 3) {
                    state = (cv::Mat_<double>(2, 1) << unwrapped[k],
                             state.at<double>(1, 0));
                    P = cv::Mat::eye(2, 2, CV_64F) * 0.5;
                    jump_run = 0;
                    ++resets;
                    ++jump_resets;
                }
                if (std::fabs(state.at<double>(1, 0)) > 1.2) {
                    ++rate_run;
                } else {
                    rate_run = 0;
                }
                if (rate_run >= 3) {
                    state = (cv::Mat_<double>(2, 1) << unwrapped[k], 0.0);
                    P = cv::Mat::eye(2, 2, CV_64F) * 0.5;
                    rate_run = 0;
                    ++resets;
                    ++rate_resets;
                }
            }
            const cv::Mat S = H * P * H.t() + r;
            const cv::Mat K = P * H.t() * S.inv();
            const cv::Mat residual =
                (cv::Mat_<double>(1, 1) << unwrapped[k]) - H * state;
            state = state + K * residual;
            P = (cv::Mat::eye(2, 2, CV_64F) - K * H) * P;
            missing_run = 0;
        } else {
            ++dropouts;
            ++missing_run;
            if (missing_run >= 5) {
                lost = true;
            }
        }
        filtered[k] = state.at<double>(0, 0);
        smoothed_value = 0.7 * smoothed_value + 0.3 * filtered[k];
        smoothed[k] = smoothed_value;

        if (k + 30 < count) {
            const double predicted =
                filtered[k] + state.at<double>(1, 0) * 1.0;
            prediction_error += std::fabs(predicted - truth[k + 30]);
            ++prediction_count;
        }
    }

    double measured_sq = 0.0;
    double averaged_sq = 0.0;
    double filtered_sq = 0.0;
    double smoothed_sq = 0.0;
    int measured_count = 0;
    for (int k = 0; k < count; ++k) {
        if (available[k]) {
            const double e = raw[k] - wrapAngle(truth[k]);
            measured_sq += e * e;
            ++measured_count;
        }
        const double ea = averaged[k] - truth[k];
        const double ef = filtered[k] - truth[k];
        const double es = smoothed[k] - truth[k];
        averaged_sq += ea * ea;
        filtered_sq += ef * ef;
        smoothed_sq += es * es;
    }

    cv::Mat canvas = makePlot(
        1200, 640, count * dt, 5.0,
        "rotating target: unwrap, filter and one-second prediction");
    const int left_margin = 95;
    const int right_margin = 45;
    const int top_margin = 70;
    const int bottom_margin = 70;
    const int plot_width = 1200;
    const int plot_height = 640;
    const int band_left = left_margin + cvRound(
        (plot_width - left_margin - right_margin) * 70 * dt / (count * dt));
    const int band_right = left_margin + cvRound(
        (plot_width - left_margin - right_margin) * 80 * dt / (count * dt));
    cv::rectangle(canvas, cv::Rect(band_left, top_margin,
                                   band_right - band_left,
                                   plot_height - top_margin - bottom_margin),
                  cv::Scalar(232, 232, 232), cv::FILLED);
    std::vector<cv::Point> true_points;
    std::vector<cv::Point> filtered_points;
    std::vector<cv::Point> smoothed_points;
    for (int k = 0; k < count; ++k) {
        const double x = k * dt;
        true_points.push_back(toPixel(x, truth[k], count * dt, 5.0,
                                      plot_width, plot_height));
        filtered_points.push_back(toPixel(x, filtered[k], count * dt, 5.0,
                                          plot_width, plot_height));
        smoothed_points.push_back(toPixel(x, smoothed[k], count * dt, 5.0,
                                          plot_width, plot_height));
        if (available[k]) {
            cv::circle(canvas, toPixel(x, unwrapped[k], count * dt, 5.0,
                                       plot_width, plot_height),
                       3, cv::Scalar(150, 150, 150), cv::FILLED,
                       cv::LINE_AA);
        }
    }
    cv::polylines(canvas, true_points, false, cv::Scalar(200, 90, 40), 2,
                  cv::LINE_AA);
    cv::polylines(canvas, smoothed_points, false, cv::Scalar(40, 170, 60), 2,
                  cv::LINE_AA);
    cv::polylines(canvas, filtered_points, false, cv::Scalar(40, 60, 220), 3,
                  cv::LINE_AA);
    legend(canvas, 700, 105, cv::Scalar(200, 90, 40), "truth");
    legend(canvas, 840, 105, cv::Scalar(150, 150, 150), "measurement");
    legend(canvas, 700, 140, cv::Scalar(40, 60, 220), "filtered");
    legend(canvas, 840, 140, cv::Scalar(40, 170, 60), "smoothed");
    cv::putText(canvas, "dropout", cv::Point(band_left + 8, top_margin + 24),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(90, 90, 90), 1,
                cv::LINE_AA);
    const bool saved_curve =
        cv::imwrite("../素材/synth_kalman_rot.png", canvas);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "frames = " << count << '\n';
    std::cout << "measured_rmse = "
              << std::sqrt(measured_sq / measured_count) << '\n';
    std::cout << "averaged_rmse = " << std::sqrt(averaged_sq / count) << '\n';
    std::cout << "filtered_rmse = " << std::sqrt(filtered_sq / count) << '\n';
    std::cout << "smoothed_rmse = " << std::sqrt(smoothed_sq / count) << '\n';
    std::cout << "prediction_mae = "
              << prediction_error / prediction_count << '\n';
    std::cout << "dropouts = " << dropouts << " resets = " << resets
              << " (lost=" << lost_resets << " jump=" << jump_resets
              << " rate=" << rate_resets << ")\n";
    std::cout << "saved synth_kalman_rot.png " << canvas.cols << "x"
              << canvas.rows << ": " << (saved_curve ? "yes" : "no") << '\n';
    return 0;
}
