#include <iostream>

#include <opencv2/core.hpp>

int main() {
    cv::Mat image(2, 3, CV_8UC3, cv::Scalar(10, 20, 30));

    std::cout << "rows=" << image.rows
              << " cols=" << image.cols
              << " channels=" << image.channels()
              << " type=" << image.type() << '\n';
    std::cout << "first pixel: " << image.at<cv::Vec3b>(0, 0) << '\n';

    return 0;
}
