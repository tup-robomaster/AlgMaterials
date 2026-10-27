#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>

int main() {
    cv::Mat depth(5, 5, CV_32F);
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            depth.at<float>(row, col) =
                static_cast<float>(1.0 + 0.1 * row + 0.01 * col);
        }
    }
    depth.at<float>(2, 2) = 0.0f;
    depth.at<float>(3, 3) = 0.0f;

    const cv::Rect roi(1, 1, 3, 3);
    const cv::Mat patch = depth(roi);
    double sum = 0.0;
    int valid = 0;
    float min_depth = 1000.0f;
    float max_depth = 0.0f;
    for (int row = 0; row < patch.rows; ++row) {
        for (int col = 0; col < patch.cols; ++col) {
            const float value = patch.at<float>(row, col);
            if (value <= 0.0f) {
                continue;
            }
            sum += value;
            min_depth = std::min(min_depth, value);
            max_depth = std::max(max_depth, value);
            ++valid;
        }
    }

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "roi = (" << roi.x << ", " << roi.y << ", "
              << roi.width << ", " << roi.height << ")\n";
    std::cout << "valid = " << valid << " of " << patch.total() << '\n';
    std::cout << "mean = " << sum / valid << '\n';
    std::cout << "min = " << min_depth << " max = " << max_depth << '\n';
    return 0;
}
