#include <iostream>

#include <opencv2/core.hpp>

int main() {
    cv::Mat image(3, 4, CV_8UC3, cv::Scalar(5, 6, 7));

    cv::Mat alias = image;
    cv::Mat copy = image.clone();

    alias.at<cv::Vec3b>(1, 2)[1] = 200;
    copy.at<cv::Vec3b>(2, 3)[2] = 150;

    const cv::Vec3b a = image.at<cv::Vec3b>(1, 2);
    std::cout << "image(1,2): [" << static_cast<int>(a[0]) << ", "
              << static_cast<int>(a[1]) << ", "
              << static_cast<int>(a[2]) << "]\n";

    const cv::Vec3b c = image.at<cv::Vec3b>(2, 3);
    std::cout << "image(2,3): [" << static_cast<int>(c[0]) << ", "
              << static_cast<int>(c[1]) << ", "
              << static_cast<int>(c[2]) << "]\n";

    std::cout << "alias shares: "
              << (alias.data == image.data ? "yes" : "no") << '\n';
    std::cout << "copy shares: "
              << (copy.data == image.data ? "yes" : "no") << '\n';

    return 0;
}
