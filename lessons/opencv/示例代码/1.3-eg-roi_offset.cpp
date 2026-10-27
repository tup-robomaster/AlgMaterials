#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：../素材/synth_balls.png" << '\n';
        return 1;
    }

    const cv::Rect rect(300, 200, 120, 90);
    cv::Mat roi = image(rect);
    std::cout << "roi rows=" << roi.rows << " cols=" << roi.cols << '\n';

    cv::Size whole;
    cv::Point offset;
    roi.locateROI(whole, offset);
    std::cout << "offset=(" << offset.x << ", " << offset.y << ") whole="
              << whole.width << "x" << whole.height << '\n';

    roi.at<cv::Vec3b>(7, 5) = cv::Vec3b(0, 0, 255);
    const cv::Vec3b changed = image.at<cv::Vec3b>(207, 305);
    std::cout << "image(207,305): [" << static_cast<int>(changed[0]) << ", "
              << static_cast<int>(changed[1]) << ", "
              << static_cast<int>(changed[2]) << "]\n";

    cv::Mat patch = roi.clone();
    patch.at<cv::Vec3b>(7, 5) = cv::Vec3b(0, 255, 0);
    const cv::Vec3b after = image.at<cv::Vec3b>(207, 305);
    std::cout << "after clone edit image(207,305): ["
              << static_cast<int>(after[0]) << ", "
              << static_cast<int>(after[1]) << ", "
              << static_cast<int>(after[2]) << "]\n";

    return 0;
}
