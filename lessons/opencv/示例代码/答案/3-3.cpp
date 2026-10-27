#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(18, 18, CV_8UC1);
    cv::rectangle(mask, cv::Rect(3, 3, 10, 4), cv::Scalar(255), cv::FILLED);

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
