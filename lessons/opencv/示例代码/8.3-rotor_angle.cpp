#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

constexpr int kWidth = 640;
constexpr int kHeight = 480;
const double kPi = 3.14159265358979323846;
const cv::Point2f kCenter(320.0f, 240.0f);

cv::Mat makeRotor() {
    cv::Mat image(kHeight, kWidth, CV_8UC3, cv::Scalar(28, 30, 34));
    for (int i = 0; i < 5; ++i) {
        const double a = i * 2.0 * kPi / 5.0;
        const double ca = std::cos(a);
        const double sa = std::sin(a);
        const double root = 55.0;
        const double tip = 178.0;
        const double root_half = 13.0;
        const double tip_half = 25.0;
        std::vector<cv::Point> blade;
        const double radii[4] = {root, root, tip, tip};
        const double halves[4] = {-root_half, root_half, tip_half, -tip_half};
        for (int j = 0; j < 4; ++j) {
            const double x = radii[j] * ca - halves[j] * sa;
            const double y = radii[j] * sa + halves[j] * ca;
            blade.push_back(cv::Point(cvRound(kCenter.x + x),
                                      cvRound(kCenter.y + y)));
        }
        cv::fillConvexPoly(image, blade, cv::Scalar(150, 150, 155),
                           cv::LINE_AA);
        cv::polylines(image, blade, true, cv::Scalar(80, 80, 86), 2,
                      cv::LINE_AA);
    }
    cv::circle(image, kCenter, 48, cv::Scalar(92, 92, 98), cv::FILLED,
               cv::LINE_AA);
    cv::circle(image, kCenter, 12, cv::Scalar(45, 45, 50), cv::FILLED,
               cv::LINE_AA);
    cv::circle(image, cv::Point(cvRound(kCenter.x + 136.0f),
                                cvRound(kCenter.y)), 11,
               cv::Scalar(30, 30, 235), cv::FILLED, cv::LINE_AA);
    return image;
}

cv::Mat rotateRotor(const cv::Mat& base, double angle) {
    const cv::Mat m = cv::getRotationMatrix2D(
        kCenter, -angle * 180.0 / kPi, 1.0);
    cv::Mat rotated;
    cv::warpAffine(base, rotated, m, base.size(), cv::INTER_LINEAR,
                   cv::BORDER_CONSTANT, cv::Scalar(28, 30, 34));
    return rotated;
}

bool fitCircle(const std::vector<cv::Point>& points, cv::Point2f& center,
               double& radius) {
    if (points.size() < 3) {
        return false;
    }
    double mean_x = 0.0;
    double mean_y = 0.0;
    for (const cv::Point& p : points) {
        mean_x += p.x;
        mean_y += p.y;
    }
    mean_x /= points.size();
    mean_y /= points.size();

    cv::Mat A(static_cast<int>(points.size()), 3, CV_64F);
    cv::Mat b(static_cast<int>(points.size()), 1, CV_64F);
    for (std::size_t i = 0; i < points.size(); ++i) {
        const double u = points[i].x - mean_x;
        const double v = points[i].y - mean_y;
        A.at<double>(static_cast<int>(i), 0) = u;
        A.at<double>(static_cast<int>(i), 1) = v;
        A.at<double>(static_cast<int>(i), 2) = 1.0;
        b.at<double>(static_cast<int>(i), 0) = u * u + v * v;
    }
    cv::Mat solution;
    if (!cv::solve(A, b, solution, cv::DECOMP_SVD)) {
        return false;
    }
    const double cx = mean_x + solution.at<double>(0, 0) / 2.0;
    const double cy = mean_y + solution.at<double>(1, 0) / 2.0;
    const double constant = solution.at<double>(2, 0);
    const double a = solution.at<double>(0, 0);
    const double b_value = solution.at<double>(1, 0);
    const double r2 = constant + a * a / 4.0 + b_value * b_value / 4.0;
    if (r2 <= 0.0) {
        return false;
    }
    center = cv::Point2f(static_cast<float>(cx), static_cast<float>(cy));
    radius = std::sqrt(r2);
    return true;
}

bool estimateCenter(const cv::Mat& frame, cv::Point2f& center,
                    double& radius) {
    cv::Mat mask;
    cv::inRange(frame, cv::Scalar(55, 55, 55), cv::Scalar(210, 210, 210),
                mask);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_NONE);
    if (contours.empty()) {
        return false;
    }
    std::vector<cv::Point> points;
    for (const std::vector<cv::Point>& contour : contours) {
        if (cv::contourArea(contour) > 100.0) {
            points.insert(points.end(), contour.begin(), contour.end());
        }
    }
    return fitCircle(points, center, radius);
}

bool measureAngle(const cv::Mat& frame, const cv::Point2f& center,
                  double& angle, cv::Point2f& marker) {
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    cv::Mat low;
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(0, 120, 100), cv::Scalar(10, 255, 255), low);
    cv::inRange(hsv, cv::Scalar(170, 120, 100), cv::Scalar(179, 255, 255),
                high);
    cv::Mat mask;
    cv::bitwise_or(low, high, mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               cv::Size(3, 3)));
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    double best_area = 0.0;
    bool found = false;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        if (area < 60.0 || area <= best_area) {
            continue;
        }
        const cv::Moments m = cv::moments(contour);
        if (m.m00 <= 0.0) {
            continue;
        }
        marker = cv::Point2f(static_cast<float>(m.m10 / m.m00),
                             static_cast<float>(m.m01 / m.m00));
        angle = std::atan2(static_cast<double>(marker.y - center.y),
                           static_cast<double>(marker.x - center.x));
        best_area = area;
        found = true;
    }
    return found;
}

void drawMeasurementPanel(cv::Mat& panel, const cv::Mat& frame,
                          const std::string& label, bool detected,
                          const cv::Point2f& center, const cv::Point2f& marker,
                          double angle) {
    frame.copyTo(panel);
    cv::drawMarker(panel, center, cv::Scalar(0, 220, 0), cv::MARKER_CROSS,
                   18, 2, cv::LINE_AA);
    if (detected) {
        cv::circle(panel, marker, 14, cv::Scalar(0, 220, 0), 2, cv::LINE_AA);
        cv::line(panel, center, marker, cv::Scalar(0, 220, 0), 2,
                 cv::LINE_AA);
        cv::putText(panel, cv::format("angle=%.3f rad", angle),
                    cv::Point(14, 30), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                    cv::Scalar(240, 240, 240), 2, cv::LINE_AA);
    }
    cv::putText(panel, label, cv::Point(14, 58),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(220, 220, 220), 2,
                cv::LINE_AA);
}

}  // namespace

int main() {
    const cv::Mat base = makeRotor();
    cv::Point2f fitted_center = kCenter;
    double fitted_radius = 0.0;
    const bool center_ok = estimateCenter(base, fitted_center, fitted_radius);

    const double sample_angle = 0.700;
    const cv::Mat sample = rotateRotor(base, sample_angle);
    double measured_sample = 0.0;
    cv::Point2f sample_marker(0.0f, 0.0f);
    const bool sample_ok = measureAngle(sample, fitted_center, measured_sample,
                                       sample_marker);

    cv::Mat measure_canvas(kHeight, 2 * kWidth, CV_8UC3,
                           cv::Scalar(20, 20, 22));
    cv::Mat left = measure_canvas(cv::Rect(0, 0, kWidth, kHeight));
    cv::Mat right = measure_canvas(cv::Rect(kWidth, 0, kWidth, kHeight));
    drawMeasurementPanel(left, base, "base", false, fitted_center,
                         sample_marker, 0.0);
    drawMeasurementPanel(right, sample, "rotated", sample_ok, fitted_center,
                         sample_marker, measured_sample);
    const bool saved_measure =
        cv::imwrite("../素材/out_rotor_measure.png", measure_canvas);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "center_ok = " << (center_ok ? "yes" : "no") << '\n';
    std::cout << "center_fit = (" << fitted_center.x << ", "
              << fitted_center.y << ")\n";
    std::cout << "center_error = "
              << std::hypot(fitted_center.x - kCenter.x,
                            fitted_center.y - kCenter.y) << '\n';
    std::cout << "fit_radius = " << fitted_radius << '\n';
    std::cout << "sample_measured = " << measured_sample << '\n';
    std::cout << "sample_error = " << measured_sample - sample_angle << '\n';
    std::cout << "saved out_rotor_measure.png " << measure_canvas.cols << "x"
              << measure_canvas.rows << ": "
              << (saved_measure ? "yes" : "no") << '\n';
    return 0;
}
