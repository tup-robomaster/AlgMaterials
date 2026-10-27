#include <algorithm>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

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

int greenCount(const cv::Mat& hsv, int s_min, int v_min) {
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(35, s_min, v_min),
                cv::Scalar(85, 255, 255), mask);
    return cv::countNonZero(mask);
}

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    const int wide = greenCount(hsv, 43, 46);
    const int s80 = greenCount(hsv, 80, 46);
    const int s120 = greenCount(hsv, 120, 46);
    const int v180 = greenCount(hsv, 43, 180);
    std::cout << "wide  S>=43 V>=46: " << wide << '\n';
    std::cout << "S>=80: " << s80 << '\n';
    std::cout << "S>=120: " << s120 << '\n';
    std::cout << "V>=180: " << v180 << '\n';

    cv::Mat mask_wide;
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(85, 255, 255),
                mask_wide);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask_wide.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::cout << "contours=" << contours.size() << '\n';

    cv::Mat mask_strict;
    cv::inRange(hsv, cv::Scalar(35, 120, 46), cv::Scalar(85, 255, 255),
                mask_strict);

    cv::Mat mask_wide_color;
    cv::cvtColor(mask_wide, mask_wide_color, cv::COLOR_GRAY2BGR);
    cv::Mat mask_strict_color;
    cv::cvtColor(mask_strict, mask_strict_color, cv::COLOR_GRAY2BGR);

    const int cell_width = 380;
    const int cell_height = 460;
    cv::Mat panel(cell_height, 3 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, mask_wide_color, cell_width, 0, cell_width, cell_height,
          "S>=43");
    place(panel, mask_strict_color, 2 * cell_width, 0, cell_width, cell_height,
          "S>=120");
    const bool saved = cv::imwrite("out_hsv_mask.png", panel);
    std::cout << "saved out_hsv_mask.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
