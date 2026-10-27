#include <algorithm>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

void print_matrix(const cv::Mat& image) {
    for (int row = 0; row < image.rows; ++row) {
        std::cout << '[';
        for (int col = 0; col < image.cols; ++col) {
            if (col != 0) {
                std::cout << ", ";
            }
            std::cout << static_cast<int>(image.at<uchar>(row, col));
        }
        std::cout << "]\n";
    }
}

}  // namespace

int main() {
    const unsigned char values[] = {0, 60, 120, 180, 240, 255, 100, 200};
    cv::Mat image(2, 4, CV_8UC1);
    std::copy(values, values + 8, image.ptr<uchar>(0));

    cv::Mat binary;
    cv::Mat truncated;
    cv::threshold(image, binary, 128, 255, cv::THRESH_BINARY);
    cv::threshold(image, truncated, 128, 255, cv::THRESH_TRUNC);

    std::cout << "BINARY 128:\n";
    print_matrix(binary);
    std::cout << "TRUNC 128:\n";
    print_matrix(truncated);
    return 0;
}
