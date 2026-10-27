#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 450.0,
        0.0, 800.0, 350.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    const cv::Vec3d rvec(0.0, 0.0, 0.0);
    const cv::Vec3d tvec(0.0, 0.0, 0.0);
    const std::vector<cv::Point3f> points = {
        cv::Point3f(0.1f, 0.2f, 1.0f),
        cv::Point3f(0.1f, 0.2f, -1.0f),
    };
    std::vector<cv::Point2f> pixels;
    cv::projectPoints(points, rvec, tvec, K, D, pixels);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "front = (" << pixels[0].x << ", " << pixels[0].y
              << ")\n";
    std::cout << "behind_center = (" << pixels[1].x << ", "
              << pixels[1].y << ")\n";
    std::cout << "front_depth = 1.000 behind_depth = -1.000\n";
    return 0;
}
