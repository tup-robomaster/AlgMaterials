#include <cmath>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

double tidy(double value) {
    return std::abs(value) < 0.0005 ? 0.0 : value;
}

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
    cv::Mat image(4, 4, CV_8UC1);
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            image.at<uchar>(row, col) =
                static_cast<uchar>(1 + 4 * row + col);
        }
    }

    cv::Mat small;
    cv::resize(image, small, cv::Size(2, 2), 0, 0, cv::INTER_NEAREST);
    std::cout << "resized=" << small.cols << 'x' << small.rows << '\n';
    print_matrix(small);

    const cv::Mat rotation =
        cv::getRotationMatrix2D(cv::Point2f(2.0F, 2.0F), 90.0, 1.0);
    std::cout << "rotation matrix:\n";
    for (int row = 0; row < rotation.rows; ++row) {
        std::cout << '['
                  << cv::format("%.1f", tidy(rotation.at<double>(row, 0)))
                  << ", "
                  << cv::format("%.1f", tidy(rotation.at<double>(row, 1)))
                  << ", "
                  << cv::format("%.1f", tidy(rotation.at<double>(row, 2)))
                  << "]\n";
    }

    return 0;
}
