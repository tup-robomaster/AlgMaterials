#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat bgra(4, 4, CV_8UC4, cv::Scalar(10, 20, 30, 128));
    const std::string path = "out_eg_alpha.png";
    const bool saved = cv::imwrite(path, bgra);
    std::cout << "saved: " << (saved ? "yes" : "no") << '\n';

    cv::Mat color = cv::imread(path, cv::IMREAD_COLOR);
    cv::Mat gray = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::Mat unchanged = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (color.empty() || gray.empty() || unchanged.empty()) {
        std::cout << "无法读取：" << path << '\n';
        return 1;
    }

    std::cout << "color channels=" << color.channels()
              << " type=" << color.type() << '\n';
    std::cout << "gray channels=" << gray.channels()
              << " type=" << gray.type() << '\n';
    std::cout << "unchanged channels=" << unchanged.channels()
              << " type=" << unchanged.type() << '\n';

    cv::Mat missing = cv::imread("not_here.png", cv::IMREAD_COLOR);
    std::cout << "missing empty: " << (missing.empty() ? "yes" : "no")
              << '\n';

    return 0;
}
