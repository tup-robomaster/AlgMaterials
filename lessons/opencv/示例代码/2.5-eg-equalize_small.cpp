#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image(4, 4, CV_8UC1);
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            image.at<uchar>(row, col) =
                static_cast<uchar>(100 + 4 * row + col);
        }
    }

    const int hist_size[] = {256};
    const float range[] = {0.0F, 256.0F};
    const float* ranges[] = {range};
    const int channels[] = {0};
    cv::Mat hist;
    cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, hist_size,
                 ranges);
    double peak = 0.0;
    cv::Point peak_bin;
    cv::minMaxLoc(hist, nullptr, &peak, nullptr, &peak_bin);
    std::cout << "hist nonzero bins=" << cv::countNonZero(hist)
              << " peak bin=" << peak_bin.y << " count="
              << static_cast<int>(peak) << '\n';

    double low_min = 0.0;
    double low_max = 0.0;
    cv::minMaxLoc(image, &low_min, &low_max);

    cv::Mat equalized;
    cv::equalizeHist(image, equalized);
    double high_min = 0.0;
    double high_max = 0.0;
    cv::minMaxLoc(equalized, &high_min, &high_max);

    std::cout << "low range=" << low_min << ".." << low_max << '\n';
    std::cout << "equalized range=" << high_min << ".." << high_max
              << '\n';
    std::cout << "equalized(1,1)="
              << static_cast<int>(equalized.at<uchar>(1, 1)) << '\n';
    std::cout << "equalized(3,3)="
              << static_cast<int>(equalized.at<uchar>(3, 3)) << '\n';
    return 0;
}
