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
    const unsigned char values[] = {10, 70, 130, 190,
                                    250, 100, 160, 220};
    cv::Mat image(2, 4, CV_8UC1);
    std::copy(values, values + 8, image.ptr<uchar>(0));

    cv::Mat inverted;
    cv::Mat tozero;
    cv::threshold(image, inverted, 150, 255, cv::THRESH_BINARY_INV);
    cv::threshold(image, tozero, 150, 255, cv::THRESH_TOZERO);

    std::cout << "BINARY_INV 150:\n";
    print_matrix(inverted);
    std::cout << "TOZERO 150:\n";
    print_matrix(tozero);
    return 0;
}
