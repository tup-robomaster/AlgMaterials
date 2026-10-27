#include <iostream>
#include <vector>

#include <opencv2/core.hpp>

int main() {
    const std::vector<uchar> values = {10, 50, 100, 200, 250};
    cv::Mat source(1, static_cast<int>(values.size()), CV_8UC1);
    for (std::size_t i = 0; i < values.size(); ++i) {
        source.at<uchar>(0, static_cast<int>(i)) = values[i];
    }

    cv::Mat gained;
    source.convertTo(gained, CV_8UC1, 1.8, 10.0);
    std::cout << "gain:";
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << " " << static_cast<int>(values[i]) << "->"
                  << static_cast<int>(gained.at<uchar>(0, static_cast<int>(i)));
    }
    std::cout << '\n';

    const std::vector<ushort> raw = {0, 512, 2048, 4095};
    cv::Mat mono12(1, static_cast<int>(raw.size()), CV_16UC1);
    for (std::size_t i = 0; i < raw.size(); ++i) {
        mono12.at<ushort>(0, static_cast<int>(i)) = raw[i];
    }

    cv::Mat mono8;
    mono12.convertTo(mono8, CV_8UC1, 255.0 / 4095.0);
    std::cout << "12bit:";
    for (std::size_t i = 0; i < raw.size(); ++i) {
        std::cout << " " << raw[i] << "->"
                  << static_cast<int>(mono8.at<uchar>(0, static_cast<int>(i)));
    }
    std::cout << '\n';
    return 0;
}
