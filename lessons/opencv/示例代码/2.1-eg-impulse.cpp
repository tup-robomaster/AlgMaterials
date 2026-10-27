#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image(5, 5, CV_8UC1, cv::Scalar(100));
    image.at<uchar>(2, 2) = 255;

    cv::Mat median;
    cv::Mat gaussian;
    cv::medianBlur(image, median, 3);
    cv::GaussianBlur(image, gaussian, cv::Size(3, 3), 0);

    std::cout << "median center="
              << static_cast<int>(median.at<uchar>(2, 2)) << '\n';
    std::cout << "gaussian center="
              << static_cast<int>(gaussian.at<uchar>(2, 2)) << '\n';
    std::cout << "median corner="
              << static_cast<int>(median.at<uchar>(0, 0)) << '\n';
    std::cout << "gaussian corner="
              << static_cast<int>(gaussian.at<uchar>(0, 0)) << '\n';
    return 0;
}
