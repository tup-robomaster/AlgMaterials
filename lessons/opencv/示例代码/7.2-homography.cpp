#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const std::vector<cv::Point2f> source = {
        cv::Point2f(0.0f, 0.0f),
        cv::Point2f(1.0f, 0.0f),
        cv::Point2f(1.0f, 1.0f),
        cv::Point2f(0.0f, 1.0f),
    };
    const std::vector<cv::Point2f> target = {
        cv::Point2f(100.0f, 100.0f),
        cv::Point2f(400.0f, 120.0f),
        cv::Point2f(380.0f, 400.0f),
        cv::Point2f(120.0f, 380.0f),
    };

    const cv::Mat H = cv::getPerspectiveTransform(source, target);
    std::vector<cv::Point2f> center = {cv::Point2f(0.5f, 0.5f)};
    std::vector<cv::Point2f> mapped;
    cv::perspectiveTransform(center, mapped, H);

    const cv::Mat p = (cv::Mat_<double>(3, 1) << 0.5, 0.5, 1.0);
    const cv::Mat q = H * p;
    const double w = q.at<double>(2, 0);
    const double manual_x = q.at<double>(0, 0) / w;
    const double manual_y = q.at<double>(1, 0) / w;

    cv::Mat H2 = cv::findHomography(source, target);
    cv::Mat Hn = H.clone();
    Hn /= Hn.at<double>(2, 2);
    H2 /= H2.at<double>(2, 2);
    double max_diff = 0.0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            max_diff = std::max(max_diff,
                                std::fabs(Hn.at<double>(row, col) -
                                          H2.at<double>(row, col)));
        }
    }

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "H =\n" << H << '\n';
    std::cout << "center = (" << mapped[0].x << ", " << mapped[0].y
              << ")\n";
    std::cout << "manual = (" << manual_x << ", " << manual_y << ")\n";
    std::cout << "difference = (" << mapped[0].x - manual_x << ", "
              << mapped[0].y - manual_y << ")\n";
    std::cout << std::setprecision(6);
    std::cout << "findHomography max diff = " << max_diff << '\n';
    return 0;
}
