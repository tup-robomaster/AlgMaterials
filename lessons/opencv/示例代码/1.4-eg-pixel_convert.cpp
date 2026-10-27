#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

void print_hsv(const std::string& name, const cv::Scalar& bgr) {
    cv::Mat pixel(1, 1, CV_8UC3, bgr);
    cv::Mat hsv;
    cv::cvtColor(pixel, hsv, cv::COLOR_BGR2HSV);
    const cv::Vec3b value = hsv.at<cv::Vec3b>(0, 0);
    std::cout << name << " BGR=[" << static_cast<int>(bgr[0]) << ", "
              << static_cast<int>(bgr[1]) << ", "
              << static_cast<int>(bgr[2]) << "] H="
              << static_cast<int>(value[0]) << " S="
              << static_cast<int>(value[1]) << " V="
              << static_cast<int>(value[2]) << '\n';
}

}  // namespace

int main() {
    const cv::Mat sample(1, 1, CV_8UC3, cv::Scalar(10, 100, 200));

    cv::Mat gray;
    cv::cvtColor(sample, gray, cv::COLOR_BGR2GRAY);
    cv::Mat hsv;
    cv::cvtColor(sample, hsv, cv::COLOR_BGR2HSV);
    const cv::Vec3b value = hsv.at<cv::Vec3b>(0, 0);

    std::cout << "sample BGR=[10, 100, 200] gray="
              << static_cast<int>(gray.at<uchar>(0, 0)) << '\n';
    std::cout << "sample HSV=[" << static_cast<int>(value[0]) << ", "
              << static_cast<int>(value[1]) << ", "
              << static_cast<int>(value[2]) << "]\n";

    print_hsv("blue", cv::Scalar(255, 0, 0));
    print_hsv("green", cv::Scalar(0, 255, 0));
    print_hsv("red", cv::Scalar(0, 0, 255));

    return 0;
}
