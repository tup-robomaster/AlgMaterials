#include <iomanip>
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

    const cv::Vec3b bgr = image.at<cv::Vec3b>(562, 151);
    std::cout << "BGR at (562,151): ["
              << static_cast<int>(bgr[0]) << ", "
              << static_cast<int>(bgr[1]) << ", "
              << static_cast<int>(bgr[2]) << "]\n";

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    std::cout << "gray at (562,151): "
              << static_cast<int>(gray.at<uchar>(562, 151)) << '\n';

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    const cv::Vec3b hsv_pixel = hsv.at<cv::Vec3b>(562, 151);
    std::cout << "HSV at (562,151): ["
              << static_cast<int>(hsv_pixel[0]) << ", "
              << static_cast<int>(hsv_pixel[1]) << ", "
              << static_cast<int>(hsv_pixel[2]) << "]\n";

    const cv::Scalar mean_bgr = cv::mean(image);
    std::cout << std::fixed << std::setprecision(2)
              << "mean BGR: [" << mean_bgr[0] << ", " << mean_bgr[1]
              << ", " << mean_bgr[2] << "]\n";

    const bool saved = cv::imwrite("out_gray_1_4.png", gray);
    std::cout << "saved out_gray_1_4.png: " << (saved ? "yes" : "no")
              << '\n';

    return 0;
}
