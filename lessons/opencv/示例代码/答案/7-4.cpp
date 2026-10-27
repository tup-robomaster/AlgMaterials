#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const cv::Mat image = cv::imread("../素材/synth_chess_3.png");
    if (image.empty()) {
        std::cerr << "无法读取 synth_chess_3.png\n";
        return 1;
    }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::Point2f> corners;
    const bool found = cv::findChessboardCorners(
        gray, cv::Size(9, 6), corners,
        cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
    std::cout << "found = " << (found ? "yes" : "no") << '\n';
    if (!found) {
        return 1;
    }
    cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                     cv::TermCriteria(cv::TermCriteria::COUNT +
                                          cv::TermCriteria::EPS,
                                      30, 0.01));
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "count = " << corners.size() << '\n';
    std::cout << "first = (" << corners.front().x << ", "
              << corners.front().y << ")\n";
    std::cout << "last = (" << corners.back().x << ", "
              << corners.back().y << ")\n";
    return 0;
}
