#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(12, 12, CV_8UC1);
    cv::rectangle(mask, cv::Rect(2, 2, 6, 2), cv::Scalar(255), cv::FILLED);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    const double area = cv::contourArea(contours.front());
    const double perimeter = cv::arcLength(contours.front(), true);
    const cv::Rect box = cv::boundingRect(contours.front());

    std::cout << "pixels=" << cv::countNonZero(mask) << '\n';
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "area=" << area << " perimeter=" << perimeter << '\n';
    std::cout << "box=(" << box.x << ", " << box.y << ", " << box.width
              << ", " << box.height << ")\n";
    std::cout << "extent="
              << area / static_cast<double>(box.width * box.height) << '\n';
    return 0;
}
