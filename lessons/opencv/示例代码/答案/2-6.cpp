#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image(5, 5, CV_8UC1);
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            image.at<uchar>(row, col) =
                static_cast<uchar>(1 + 5 * row + col);
        }
    }

    const cv::Mat rotation =
        cv::getRotationMatrix2D(cv::Point2f(2.0F, 2.0F), 180.0, 1.0);
    cv::Mat rotated;
    cv::warpAffine(image, rotated, rotation, cv::Size(5, 5),
                   cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0));

    std::cout << "rotated 180:\n";
    for (int row = 0; row < rotated.rows; ++row) {
        std::cout << '[';
        for (int col = 0; col < rotated.cols; ++col) {
            if (col != 0) {
                std::cout << ", ";
            }
            std::cout << static_cast<int>(rotated.at<uchar>(row, col));
        }
        std::cout << "]\n";
    }
    return 0;
}
