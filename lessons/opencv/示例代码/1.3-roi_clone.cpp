#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：../素材/synth_balls.png" << '\n';
        return 1;
    }

    const cv::Vec3b pixel = image.at<cv::Vec3b>(100, 200);
    std::cout << "pixel(100,200): ["
              << static_cast<int>(pixel[0]) << ", "
              << static_cast<int>(pixel[1]) << ", "
              << static_cast<int>(pixel[2]) << "]\n";

    const cv::Rect rect(100, 100, 200, 150);
    cv::Mat roi = image(rect);
    std::cout << "roi: rows=" << roi.rows << " cols=" << roi.cols
              << " at (" << rect.x << ", " << rect.y << ")\n";

    roi.at<cv::Vec3b>(10, 10) = cv::Vec3b(0, 0, 255);
    const cv::Vec3b changed = image.at<cv::Vec3b>(110, 110);
    std::cout << "after roi change, image(110,110): ["
              << static_cast<int>(changed[0]) << ", "
              << static_cast<int>(changed[1]) << ", "
              << static_cast<int>(changed[2]) << "]\n";

    cv::Mat patch = roi.clone();
    std::cout << "clone shares data: "
              << (patch.data == roi.data ? "yes" : "no") << '\n';

    patch.at<cv::Vec3b>(0, 0) = cv::Vec3b(0, 255, 0);
    const cv::Vec3b after_clone = image.at<cv::Vec3b>(100, 100);
    std::cout << "after clone change, image(100,100): ["
              << static_cast<int>(after_clone[0]) << ", "
              << static_cast<int>(after_clone[1]) << ", "
              << static_cast<int>(after_clone[2]) << "]\n";

    cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 3);
    const bool saved = cv::imwrite("out_roi.png", image);
    std::cout << "saved out_roi.png: " << (saved ? "yes" : "no") << '\n';

    return 0;
}
