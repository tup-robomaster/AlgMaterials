#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

double tidy(double value) {
    return std::abs(value) < 0.0005 ? 0.0 : value;
}

void print_matrix(const cv::Mat& matrix) {
    for (int row = 0; row < matrix.rows; ++row) {
        std::cout << '[';
        for (int col = 0; col < matrix.cols; ++col) {
            if (col != 0) {
                std::cout << ", ";
            }
            std::cout << cv::format("%.3f", tidy(matrix.at<double>(row, col)));
        }
        std::cout << "]\n";
    }
}

void place(cv::Mat& panel, const cv::Mat& image, int x, int y, int width,
           int height, const std::string& label) {
    const int label_height = 28;
    cv::rectangle(panel, cv::Rect(x, y, width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);

    const int inner_width = width - 16;
    const int inner_height = height - label_height - 16;
    const double scale = std::min(static_cast<double>(inner_width) / image.cols,
                                  static_cast<double>(inner_height) / image.rows);
    const int shown_width = std::max(1, cvRound(image.cols * scale));
    const int shown_height = std::max(1, cvRound(image.rows * scale));
    cv::Mat shown;
    cv::resize(image, shown, cv::Size(shown_width, shown_height), 0, 0,
               cv::INTER_AREA);
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

}  // namespace

int main() {
    const std::string balls_path = "../素材/synth_balls.png";
    const std::string card_path = "../素材/synth_card.png";
    cv::Mat balls = cv::imread(balls_path, cv::IMREAD_COLOR);
    cv::Mat card = cv::imread(card_path, cv::IMREAD_COLOR);
    if (balls.empty() || card.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat half;
    cv::resize(balls, half, cv::Size(balls.cols / 2, balls.rows / 2), 0, 0,
               cv::INTER_AREA);
    std::cout << "resize half=" << half.cols << 'x' << half.rows << '\n';

    const cv::Point2f center(balls.cols / 2.0F, balls.rows / 2.0F);
    const cv::Mat rotation =
        cv::getRotationMatrix2D(center, 30.0, 1.0);
    cv::Mat rotated;
    cv::warpAffine(balls, rotated, rotation, balls.size(), cv::INTER_LINEAR,
                   cv::BORDER_CONSTANT, cv::Scalar(235, 235, 235));
    std::cout << "rotation matrix:\n";
    print_matrix(rotation);

    const cv::Point2f source[4] = {
        cv::Point2f(180.0F, 120.0F), cv::Point2f(720.0F, 180.0F),
        cv::Point2f(760.0F, 540.0F), cv::Point2f(140.0F, 500.0F)};
    const cv::Point2f target[4] = {
        cv::Point2f(0.0F, 0.0F), cv::Point2f(599.0F, 0.0F),
        cv::Point2f(599.0F, 399.0F), cv::Point2f(0.0F, 399.0F)};
    const cv::Mat perspective = cv::getPerspectiveTransform(source, target);
    cv::Mat corrected;
    cv::warpPerspective(card, corrected, perspective, cv::Size(600, 400),
                        cv::INTER_LINEAR, cv::BORDER_CONSTANT,
                        cv::Scalar(255, 255, 255));
    std::cout << "perspective matrix:\n";
    print_matrix(perspective);

    const bool resize_saved = cv::imwrite("out_resize.png", half);
    const bool rotate_saved = cv::imwrite("out_rotate.png", rotated);
    const bool warp_saved = cv::imwrite("out_warp.png", corrected);
    std::cout << "saved out_resize.png: " << (resize_saved ? "yes" : "no")
              << '\n';
    std::cout << "saved out_rotate.png: " << (rotate_saved ? "yes" : "no")
              << '\n';
    std::cout << "saved out_warp.png: " << (warp_saved ? "yes" : "no")
              << '\n';

    const int cell_width = 420;
    const int cell_height = 468;
    cv::Mat panel(cell_height, 3 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, half, 0, 0, cell_width, cell_height, "resize half");
    place(panel, rotated, cell_width, 0, cell_width, cell_height,
          "rotate 30 deg");
    place(panel, corrected, 2 * cell_width, 0, cell_width, cell_height,
          "perspective");
    const bool panel_saved = cv::imwrite("out_geometry.png", panel);
    std::cout << "saved out_geometry.png: " << (panel_saved ? "yes" : "no")
              << '\n';
    return 0;
}
