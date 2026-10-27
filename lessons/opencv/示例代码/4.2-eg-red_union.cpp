#include <algorithm>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat hsv(100, 100, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::rectangle(hsv, cv::Rect(10, 10, 20, 20), cv::Scalar(5, 255, 255),
                  cv::FILLED);
    cv::rectangle(hsv, cv::Rect(60, 10, 6, 6), cv::Scalar(175, 255, 255),
                  cv::FILLED);

    cv::Mat bgr;
    cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
    cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);

    cv::Mat low;
    cv::inRange(hsv, cv::Scalar(0, 80, 60), cv::Scalar(10, 255, 255), low);
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(170, 80, 60), cv::Scalar(179, 255, 255),
                high);
    cv::Mat mask;
    cv::bitwise_or(low, high, mask);
    std::cout << "low=" << cv::countNonZero(low)
              << " high=" << cv::countNonZero(high)
              << " union=" << cv::countNonZero(mask) << '\n';

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<int> areas;
    for (const std::vector<cv::Point>& contour : contours) {
        areas.push_back(cvRound(cv::contourArea(contour)));
    }
    std::sort(areas.begin(), areas.end(), std::greater<int>());
    std::cout << "contours=" << contours.size() << '\n';
    for (std::size_t i = 0; i < areas.size(); ++i) {
        std::cout << "area[" << i << "]=" << areas[i] << '\n';
    }

    int kept = 0;
    for (int area : areas) {
        if (area >= 100) {
            ++kept;
        }
    }
    std::cout << "kept=" << kept << '\n';
    return 0;
}
