#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main() {
    const std::string path = "../素材/synth_balls.png";

    cv::Mat color = cv::imread(path, cv::IMREAD_COLOR);
    cv::Mat gray = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::Mat unchanged = cv::imread(path, cv::IMREAD_UNCHANGED);

    if (color.empty() || gray.empty() || unchanged.empty()) {
        std::cout << "无法读取：" << path << std::endl;
        return 1;
    }

    std::cout << "color: rows=" << color.rows << " cols=" << color.cols
              << " channels=" << color.channels()
              << " type=" << color.type() << '\n';
    std::cout << "gray: rows=" << gray.rows << " cols=" << gray.cols
              << " channels=" << gray.channels()
              << " type=" << gray.type() << '\n';
    std::cout << "unchanged: rows=" << unchanged.rows
              << " cols=" << unchanged.cols
              << " channels=" << unchanged.channels()
              << " type=" << unchanged.type() << '\n';

    cv::Mat missing = cv::imread("../素材/not_here.png", cv::IMREAD_COLOR);
    std::cout << "missing empty: " << (missing.empty() ? "yes" : "no")
              << '\n';

    const bool saved = cv::imwrite("out_gray.png", gray);
    std::cout << "saved out_gray.png: " << (saved ? "yes" : "no")
              << std::endl;

    cv::imshow("gray", gray);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
