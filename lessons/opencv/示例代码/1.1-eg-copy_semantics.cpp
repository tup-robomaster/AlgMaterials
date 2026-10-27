#include <iostream>

#include <opencv2/core.hpp>

int main() {
    cv::Mat image(2, 3, CV_8UC3, cv::Scalar(10, 20, 30));

    cv::Mat alias = image;
    alias.at<cv::Vec3b>(0, 0)[0] = 9;

    cv::Mat clone = image.clone();
    clone.at<cv::Vec3b>(0, 0)[0] = 99;

    cv::Mat copied;
    image.copyTo(copied);
    copied.at<cv::Vec3b>(0, 0)[0] = 77;

    std::cout << "image type=" << image.type()
              << " channels=" << image.channels() << '\n';
    std::cout << "image shares alias: "
              << (image.data == alias.data ? "yes" : "no") << '\n';
    std::cout << "image shares clone: "
              << (image.data == clone.data ? "yes" : "no") << '\n';
    std::cout << "image shares copied: "
              << (image.data == copied.data ? "yes" : "no") << '\n';

    const cv::Vec3b original = image.at<cv::Vec3b>(0, 0);
    std::cout << "image(0,0): [" << static_cast<int>(original[0]) << ", "
              << static_cast<int>(original[1]) << ", "
              << static_cast<int>(original[2]) << "]\n";

    const cv::Vec3b cloned = clone.at<cv::Vec3b>(0, 0);
    std::cout << "clone(0,0): [" << static_cast<int>(cloned[0]) << ", "
              << static_cast<int>(cloned[1]) << ", "
              << static_cast<int>(cloned[2]) << "]\n";

    const cv::Vec3b copy = copied.at<cv::Vec3b>(0, 0);
    std::cout << "copied(0,0): [" << static_cast<int>(copy[0]) << ", "
              << static_cast<int>(copy[1]) << ", "
              << static_cast<int>(copy[2]) << "]\n";

    return 0;
}
