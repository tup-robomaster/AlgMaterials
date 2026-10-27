#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(20, 20, CV_8UC1);
    const cv::RotatedRect source(cv::Point2f(10.0F, 10.0F),
                                 cv::Size2f(8.0F, 4.0F), 30.0F);
    cv::Point2f points[4];
    source.points(points);
    std::vector<cv::Point> polygon;
    for (const cv::Point2f& point : points) {
        polygon.emplace_back(cvRound(point.x), cvRound(point.y));
    }
    cv::fillConvexPoly(mask, polygon, cv::Scalar(255), cv::LINE_8);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    const cv::RotatedRect rect = cv::minAreaRect(contours.front());
    const double long_side = std::max(rect.size.width, rect.size.height);
    const double short_side = std::min(rect.size.width, rect.size.height);
    const double perimeter = cv::arcLength(contours.front(), true);
    std::vector<cv::Point> approx;
    cv::approxPolyDP(contours.front(), approx, 0.02 * perimeter, true);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "center=(" << rect.center.x << ", " << rect.center.y
              << ") size=" << long_side << "x" << short_side << '\n';
    std::cout << "approx=" << approx.size() << '\n';
    return 0;
}
