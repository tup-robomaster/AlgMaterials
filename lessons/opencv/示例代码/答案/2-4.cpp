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
    cv::Mat image = cv::Mat::zeros(11, 11, CV_8UC1);
    cv::rectangle(image, cv::Rect(3, 3, 5, 5), cv::Scalar(255),
                  cv::FILLED);

    cv::Mat sobel_x16;
    cv::Mat sobel_y16;
    cv::Sobel(image, sobel_x16, CV_16S, 1, 0, 3);
    cv::Sobel(image, sobel_y16, CV_16S, 0, 1, 3);
    cv::Mat sobel_x;
    cv::Mat sobel_y;
    cv::convertScaleAbs(sobel_x16, sobel_x);
    cv::convertScaleAbs(sobel_y16, sobel_y);

    cv::Mat edges;
    cv::Canny(image, edges, 50, 150, 3);

    std::cout << "gx(4,3)=" << static_cast<int>(sobel_x.at<uchar>(4, 3))
              << " gy(3,4)=" << static_cast<int>(sobel_y.at<uchar>(3, 4))
              << '\n';
    std::cout << "gx >100=" << count_above(sobel_x, 100) << '\n';
    std::cout << "gy >100=" << count_above(sobel_y, 100) << '\n';
    std::cout << "canny nonzero=" << cv::countNonZero(edges) << '\n';
    return 0;
}
