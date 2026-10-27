#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 500.0,
        0.0, 800.0, 300.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = (cv::Mat_<double>(1, 5) <<
        -0.35, 0.12, 0.001, -0.0005, 0.0);
    const cv::Size size(1000, 600);

    cv::Rect roi;
    const cv::Mat new_k = cv::getOptimalNewCameraMatrix(K, D, size, 1.0,
                                                         size, &roi);
    std::vector<cv::Point2f> before = {cv::Point2f(200.0f, 150.0f)};
    std::vector<cv::Point2f> after;
    cv::undistortPoints(before, after, K, D, cv::noArray(), new_k);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "K_new =\n" << new_k << '\n';
    std::cout << "roi = (" << roi.x << ", " << roi.y << ", "
              << roi.width << ", " << roi.height << ")\n";
    std::cout << "(200, 150) -> (" << after[0].x << ", "
              << after[0].y << ")\n";
    return 0;
}
