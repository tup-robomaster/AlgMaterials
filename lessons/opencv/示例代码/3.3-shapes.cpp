#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

double side_ratio(const cv::RotatedRect& rect) {
    const double long_side = std::max(rect.size.width, rect.size.height);
    const double short_side = std::max(1.0, std::min(
        static_cast<double>(rect.size.width),
        static_cast<double>(rect.size.height)));
    return long_side / short_side;
}

int count_right_angles(const std::vector<cv::Point>& polygon) {
    int count = 0;
    const std::size_t n = polygon.size();
    for (std::size_t i = 0; i < n; ++i) {
        const cv::Point2d before(polygon[(i + n - 1) % n] - polygon[i]);
        const cv::Point2d after(polygon[(i + 1) % n] - polygon[i]);
        const double length = std::max(1e-6,
                                       std::sqrt(before.dot(before)) *
                                       std::sqrt(after.dot(after)));
        if (std::abs(before.dot(after)) / length < 0.15) {
            ++count;
        }
    }
    return count;
}

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/synth_blocks.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat gray;
    cv::Mat mask;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, mask, 160, 255, cv::THRESH_BINARY_INV);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(3, 3)));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    cv::Mat marked_rect = image.clone();
    cv::Mat marked_poly = image.clone();
    int squares = 0;
    std::cout << std::fixed << std::setprecision(2);
    for (std::size_t i = 0; i < contours.size(); ++i) {
        const cv::RotatedRect rect = cv::minAreaRect(contours[i]);
        const double perimeter = cv::arcLength(contours[i], true);
        std::vector<cv::Point> polygon;
        cv::approxPolyDP(contours[i], polygon, 0.03 * perimeter, true);
        const int right_angles = count_right_angles(polygon);
        const bool square = polygon.size() == 4 && side_ratio(rect) <= 1.12
                            && right_angles == 4;
        if (square) {
            ++squares;
        }
        std::cout << "i=" << i << " center=(" << rect.center.x << ", "
                  << rect.center.y << ") size=" << rect.size.width << "x"
                  << rect.size.height << " angle=" << rect.angle
                  << " approx=" << polygon.size()
                  << " right=" << right_angles
                  << " square=" << (square ? "yes" : "no") << '\n';

        cv::Point2f corners[4];
        rect.points(corners);
        for (int k = 0; k < 4; ++k) {
            cv::line(marked_rect, corners[k], corners[(k + 1) % 4],
                     cv::Scalar(0, 200, 0), 2);
        }
        std::vector<std::vector<cv::Point>> one{polygon};
        cv::drawContours(marked_poly, one, -1, cv::Scalar(0, 0, 255), 2);
        for (cv::Mat* canvas : {&marked_rect, &marked_poly}) {
            cv::putText(*canvas, std::to_string(i),
                        cv::Point(cvRound(rect.center.x) - 8,
                                  cvRound(rect.center.y) + 8),
                        cv::FONT_HERSHEY_SIMPLEX, 0.65,
                        cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
        }
    }
    std::cout << "squares=" << squares << " of " << contours.size() << '\n';

    cv::Mat panel(image.rows + 40, 2 * image.cols, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    marked_rect.copyTo(panel(cv::Rect(0, 40, image.cols, image.rows)));
    marked_poly.copyTo(
        panel(cv::Rect(image.cols, 40, image.cols, image.rows)));
    cv::putText(panel, "minAreaRect", cv::Point(12, 27),
                cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);
    cv::putText(panel, "approxPolyDP (4 squares)",
                cv::Point(image.cols + 12, 27), cv::FONT_HERSHEY_SIMPLEX,
                0.65, cv::Scalar(30, 30, 30), 1, cv::LINE_AA);
    const bool saved = cv::imwrite("out_shapes.png", panel);
    std::cout << "saved out_shapes.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
