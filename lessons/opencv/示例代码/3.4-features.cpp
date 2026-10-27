#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

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

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls_1.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(85, 255, 255), mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(3, 3)));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    cv::Mat marked = image.clone();
    int kept = 0;
    std::cout << std::fixed << std::setprecision(3);
    for (std::size_t i = 0; i < contours.size(); ++i) {
        const cv::Moments moments = cv::moments(contours[i]);
        if (moments.m00 <= 0.0) {
            continue;
        }
        const double area = cv::contourArea(contours[i]);
        const double perimeter = cv::arcLength(contours[i], true);
        const double circularity =
            4.0 * CV_PI * area / (perimeter * perimeter);
        const cv::Point2d center(moments.m10 / moments.m00,
                                 moments.m01 / moments.m00);
        std::cout << "i=" << i << " center=(" << center.x << ", "
                  << center.y << ") area=" << area
                  << " circularity=" << circularity;
        if (area >= 1500.0 && area <= 8000.0 &&
            circularity >= 0.72 && circularity <= 1.10) {
            std::cout << " keep\n";
            cv::circle(marked, cv::Point(cvRound(center.x),
                                         cvRound(center.y)),
                       3, cv::Scalar(0, 0, 255), cv::FILLED);
            cv::drawMarker(marked, cv::Point(cvRound(center.x),
                                             cvRound(center.y)),
                           cv::Scalar(255, 0, 0), cv::MARKER_CROSS, 16, 1);
            ++kept;
        } else {
            std::cout << " drop\n";
        }
    }
    std::cout << "kept=" << kept << " of " << contours.size() << '\n';

    const int cell_width = 430;
    const int cell_height = 500;
    cv::Mat panel(cell_height, 2 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, marked, cell_width, 0, cell_width, cell_height,
          "centroid + circularity");
    const bool saved = cv::imwrite("out_features.png", panel);
    std::cout << "saved out_features.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
