#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    cv::Mat image = cv::imread("../素材/synth_chess_1.png");
    if (image.empty()) {
        std::cerr << "无法读取 synth_chess_1.png\n";
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    const cv::Size pattern(9, 6);
    const int flags = cv::CALIB_CB_ADAPTIVE_THRESH |
                      cv::CALIB_CB_NORMALIZE_IMAGE;

    std::vector<cv::Point2f> corners;
    const bool found = cv::findChessboardCorners(gray, pattern, corners,
                                                 flags);
    std::vector<cv::Point2f> wrong;
    const bool wrong_found = cv::findChessboardCorners(
        gray, cv::Size(8, 6), wrong, flags);

    std::cout << "image = synth_chess_1.png " << image.cols << "x"
              << image.rows << '\n';
    std::cout << "pattern = " << pattern.width << "x" << pattern.height
              << " (inner corners)\n";
    std::cout << "found = " << (found ? "yes" : "no") << '\n';
    if (found) {
        std::cout << "count = " << corners.size() << '\n';
        cv::cornerSubPix(gray, corners, cv::Size(11, 11),
                         cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::COUNT +
                                              cv::TermCriteria::EPS,
                                          30, 0.01));
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "first = (" << corners.front().x << ", "
                  << corners.front().y << ")\n";
        std::cout << "last = (" << corners.back().x << ", "
                  << corners.back().y << ")\n";
        cv::drawChessboardCorners(image, pattern, corners, found);
    }
    std::cout << "wrong 8x6 found = " << (wrong_found ? "yes" : "no")
              << '\n';
    const bool saved = cv::imwrite("../素材/out_chessboard.png", image);
    std::cout << "saved out_chessboard.png " << image.cols << "x"
              << image.rows << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
