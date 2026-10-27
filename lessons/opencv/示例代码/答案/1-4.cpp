#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

void print_colors(const std::string& name, const cv::Scalar& bgr) {
    cv::Mat pixel(1, 1, CV_8UC3, bgr);
    cv::Mat gray;
    cv::Mat hsv;
    cv::cvtColor(pixel, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(pixel, hsv, cv::COLOR_BGR2HSV);
    const cv::Vec3b value = hsv.at<cv::Vec3b>(0, 0);

    std::cout << name << " BGR=[" << static_cast<int>(bgr[0]) << ", "
              << static_cast<int>(bgr[1]) << ", "
              << static_cast<int>(bgr[2]) << "] gray="
              << static_cast<int>(gray.at<uchar>(0, 0)) << " H="
              << static_cast<int>(value[0]) << " S="
              << static_cast<int>(value[1]) << " V="
              << static_cast<int>(value[2]) << '\n';
}

}  // namespace

int main() {
    print_colors("red", cv::Scalar(0, 0, 255));
    print_colors("yellow", cv::Scalar(0, 255, 255));
    print_colors("magenta", cv::Scalar(255, 0, 255));
    return 0;
}
