#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat hsv(20, 20, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::rectangle(hsv, cv::Rect(0, 0, 10, 10), cv::Scalar(5, 255, 255),
                  cv::FILLED);
    cv::rectangle(hsv, cv::Rect(10, 0, 10, 10), cv::Scalar(175, 255, 255),
                  cv::FILLED);
    cv::rectangle(hsv, cv::Rect(0, 10, 10, 10), cv::Scalar(60, 255, 255),
                  cv::FILLED);
    cv::rectangle(hsv, cv::Rect(10, 10, 10, 10), cv::Scalar(0, 0, 255),
                  cv::FILLED);

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
    std::cout << "white_in_union="
              << cv::countNonZero(mask(cv::Rect(10, 10, 10, 10)))
              << " green_in_union="
              << cv::countNonZero(mask(cv::Rect(0, 10, 10, 10))) << '\n';
    return 0;
}
