#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat bgr(40, 80, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::rectangle(bgr, cv::Rect(5, 5, 10, 10), cv::Scalar(0, 0, 255),
                  cv::FILLED);
    cv::rectangle(bgr, cv::Rect(30, 5, 10, 10), cv::Scalar(255, 0, 0),
                  cv::FILLED);
    cv::rectangle(bgr, cv::Rect(55, 5, 10, 10), cv::Scalar(0, 128, 255),
                  cv::FILLED);

    cv::Mat hsv;
    cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    cv::split(bgr, channels);

    cv::Mat red_diff;
    cv::Mat blue_diff;
    cv::subtract(channels[2], channels[0], red_diff, cv::noArray(),
                 CV_16S);
    cv::subtract(channels[0], channels[2], blue_diff, cv::noArray(),
                 CV_16S);
    cv::Mat red_by_diff;
    cv::Mat blue_by_diff;
    cv::compare(red_diff, 80, red_by_diff, cv::CMP_GT);
    cv::compare(blue_diff, 80, blue_by_diff, cv::CMP_GT);

    cv::Mat red_low;
    cv::inRange(hsv, cv::Scalar(0, 80, 60), cv::Scalar(10, 255, 255),
                red_low);
    cv::Mat red_high;
    cv::inRange(hsv, cv::Scalar(170, 80, 60), cv::Scalar(179, 255, 255),
                red_high);
    cv::Mat red_by_hsv;
    cv::bitwise_or(red_low, red_high, red_by_hsv);
    cv::Mat blue_by_hsv;
    cv::inRange(hsv, cv::Scalar(100, 80, 60), cv::Scalar(130, 255, 255),
                blue_by_hsv);

    std::cout << "R-B>80: " << cv::countNonZero(red_by_diff) << '\n';
    std::cout << "B-R>80: " << cv::countNonZero(blue_by_diff) << '\n';
    std::cout << "red HSV: " << cv::countNonZero(red_by_hsv) << '\n';
    std::cout << "blue HSV: " << cv::countNonZero(blue_by_hsv) << '\n';
    return 0;
}
