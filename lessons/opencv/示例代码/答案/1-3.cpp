#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：../素材/synth_balls.png" << '\n';
        return 1;
    }

    const cv::Rect rect(400, 500, 100, 80);
    cv::Mat roi = image(rect);
    roi.at<cv::Vec3b>(3, 4) = cv::Vec3b(255, 0, 0);

    const cv::Vec3b pixel = image.at<cv::Vec3b>(503, 404);
    std::cout << "image(503,404): [" << static_cast<int>(pixel[0]) << ", "
              << static_cast<int>(pixel[1]) << ", "
              << static_cast<int>(pixel[2]) << "]\n";

    cv::Size whole;
    cv::Point offset;
    roi.locateROI(whole, offset);
    std::cout << "offset=(" << offset.x << ", " << offset.y << ")\n";
    std::cout << "roi rows=" << roi.rows << " cols=" << roi.cols << '\n';

    return 0;
}
