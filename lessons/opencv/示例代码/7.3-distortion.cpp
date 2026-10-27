#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

namespace {

cv::Point2d distort(double x, double y, double k1, double k2) {
    const double r2 = x * x + y * y;
    const double factor = 1.0 + k1 * r2 + k2 * r2 * r2;
    return cv::Point2d(x * factor, y * factor);
}

void drawGrid(cv::Mat& panel, const cv::Mat& K, double k1, double k2,
              bool apply_distortion) {
    const cv::Scalar color(40, 40, 200);
    for (int i = -4; i <= 4; ++i) {
        const double fixed = i * 0.1;
        std::vector<cv::Point> points;
        for (int step = 0; step <= 80; ++step) {
            const double t = -0.4 + 0.8 * step / 80.0;
            cv::Point2d p = apply_distortion
                ? distort(fixed, t, k1, k2)
                : cv::Point2d(fixed, t);
            points.push_back(cv::Point(
                cvRound(K.at<double>(0, 0) * p.x + K.at<double>(0, 2)),
                cvRound(K.at<double>(1, 1) * p.y + K.at<double>(1, 2))));
        }
        cv::polylines(panel, points, false, color, 1, cv::LINE_AA);

        points.clear();
        for (int step = 0; step <= 80; ++step) {
            const double t = -0.4 + 0.8 * step / 80.0;
            cv::Point2d p = apply_distortion
                ? distort(t, fixed, k1, k2)
                : cv::Point2d(t, fixed);
            points.push_back(cv::Point(
                cvRound(K.at<double>(0, 0) * p.x + K.at<double>(0, 2)),
                cvRound(K.at<double>(1, 1) * p.y + K.at<double>(1, 2))));
        }
        cv::polylines(panel, points, false, color, 1, cv::LINE_AA);
    }
    cv::putText(panel, apply_distortion ? "barrel" : "ideal",
                cv::Point(12, 24), cv::FONT_HERSHEY_SIMPLEX, 0.7,
                cv::Scalar(30, 30, 30), 2, cv::LINE_AA);
}

}  // namespace

int main() {
    const double k1 = -0.30;
    const double k2 = 0.10;
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        600.0, 0.0, 250.0,
        0.0, 600.0, 250.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = (cv::Mat_<double>(1, 5) << k1, k2, 0.0, 0.0, 0.0);

    cv::Mat canvas(500, 1000, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat left = canvas(cv::Rect(0, 0, 500, 500));
    cv::Mat right = canvas(cv::Rect(500, 0, 500, 500));
    drawGrid(left, K, k1, k2, false);
    drawGrid(right, K, k1, k2, true);

    const bool saved = cv::imwrite("../素材/out_distortion.png", canvas);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "k1 = " << k1 << " k2 = " << k2 << '\n';
    std::cout << "r       r'      delta\n";
    for (const double r : {0.1, 0.2, 0.3, 0.4}) {
        const double rp = r * (1.0 + k1 * r * r + k2 * r * r * r * r);
        std::cout << r << "   " << rp << "  " << rp - r << '\n';
    }

    const double x = 0.3;
    const double y = 0.1;
    const cv::Point2d manual = distort(x, y, k1, k2);
    const double manual_u = K.at<double>(0, 0) * manual.x +
                            K.at<double>(0, 2);
    const double manual_v = K.at<double>(1, 1) * manual.y +
                            K.at<double>(1, 2);
    std::vector<cv::Point3f> object = {cv::Point3f(
        static_cast<float>(x), static_cast<float>(y), 0.0f)};
    std::vector<cv::Point2f> projected;
    cv::projectPoints(object, cv::Vec3d(0.0, 0.0, 0.0),
                      cv::Vec3d(0.0, 0.0, 0.0), K, D, projected);
    std::cout << "manual pixel = (" << manual_u << ", " << manual_v
              << ")\n";
    std::cout << "projectPoints = (" << projected[0].x << ", "
              << projected[0].y << ")\n";
    std::cout << "difference = (" << manual_u - projected[0].x << ", "
              << manual_v - projected[0].y << ")\n";
    std::cout << "saved out_distortion.png 1000x500: "
              << (saved ? "yes" : "no") << '\n';
    return 0;
}
