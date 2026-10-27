#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

int count_above(const cv::Mat& image, int value) {
    cv::Mat binary;
    cv::threshold(image, binary, value, 255, cv::THRESH_BINARY);
    return cv::countNonZero(binary);
}

}  // namespace

int main() {
    cv::Mat image = cv::Mat::zeros(9, 9, CV_8UC1);
    image.colRange(4, 9).setTo(cv::Scalar(255));

    cv::Mat sobel_x16;
    cv::Sobel(image, sobel_x16, CV_16S, 1, 0, 3);
    cv::Mat sobel_x;
    cv::convertScaleAbs(sobel_x16, sobel_x);

    cv::Mat edges;
    cv::Canny(image, edges, 50, 150, 3);

    std::cout << "gx(4,3)=" << static_cast<int>(sobel_x.at<uchar>(4, 3))
              << " gx(4,4)=" << static_cast<int>(sobel_x.at<uchar>(4, 4))
              << " gx(4,5)=" << static_cast<int>(sobel_x.at<uchar>(4, 5))
              << '\n';
    std::cout << "gx >100=" << count_above(sobel_x, 100) << '\n';
    std::cout << "canny nonzero=" << cv::countNonZero(edges) << '\n';
    return 0;
}
