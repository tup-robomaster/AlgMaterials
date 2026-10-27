#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

void show(const std::string& name, int blue, int green, int red) {
    cv::Mat bgr(1, 1, CV_8UC3, cv::Scalar(blue, green, red));
    cv::Mat hsv;
    cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
    const cv::Vec3b pixel = hsv.at<cv::Vec3b>(0, 0);
    const int h = pixel[0];
    const int s = pixel[1];
    const int v = pixel[2];
    const bool low = h >= 0 && h <= 10 && s >= 80 && v >= 60;
    const bool high = h >= 170 && h <= 179 && s >= 80 && v >= 60;
    std::cout << name << ": H=" << h << " S=" << s << " V=" << v
              << " low=" << (low ? "yes" : "no")
              << " high=" << (high ? "yes" : "no") << '\n';
}

}  // namespace

int main() {
    show("red", 0, 0, 255);
    show("orange-red", 0, 64, 255);
    show("magenta-red", 32, 0, 255);
    show("white", 255, 255, 255);
    show("pink", 180, 180, 255);
    show("gray", 128, 128, 128);
    return 0;
}
