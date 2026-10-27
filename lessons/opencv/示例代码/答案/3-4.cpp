#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(12, 12, CV_8UC1);
    cv::rectangle(mask, cv::Rect(2, 2, 4, 6), cv::Scalar(255), cv::FILLED);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    const cv::Moments moments = cv::moments(contours.front());
    const double area = cv::contourArea(contours.front());
    const double perimeter = cv::arcLength(contours.front(), true);
    const double circularity =
        4.0 * CV_PI * area / (perimeter * perimeter);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "m00=" << moments.m00 << '\n';
    std::cout << "centroid=(" << moments.m10 / moments.m00 << ", "
              << moments.m01 / moments.m00 << ")\n";
    std::cout << "circularity=" << circularity << '\n';
    return 0;
}
