#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image(7, 7, CV_8UC1, cv::Scalar(80));
    image.at<uchar>(3, 3) = 0;
    image.at<uchar>(3, 4) = 255;

    cv::Mat median;
    cv::Mat gaussian;
    cv::medianBlur(image, median, 3);
    cv::GaussianBlur(image, gaussian, cv::Size(3, 3), 0);

    std::cout << "median pair: ["
              << static_cast<int>(median.at<uchar>(3, 3)) << ", "
              << static_cast<int>(median.at<uchar>(3, 4)) << "]\n";
    std::cout << "gaussian pair: ["
              << static_cast<int>(gaussian.at<uchar>(3, 3)) << ", "
              << static_cast<int>(gaussian.at<uchar>(3, 4)) << "]\n";
    return 0;
}
