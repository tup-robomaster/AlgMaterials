#include <algorithm>
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

cv::Mat redMask(const cv::Mat& hsv, bool with_high_side) {
    cv::Mat low;
    cv::inRange(hsv, cv::Scalar(0, 80, 60), cv::Scalar(10, 255, 255), low);
    if (!with_high_side) {
        return low;
    }
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(170, 80, 60), cv::Scalar(179, 255, 255),
                high);
    cv::Mat mask;
    cv::bitwise_or(low, high, mask);
    return mask;
}

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/apple.jpg", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    const cv::Mat low = redMask(hsv, false);
    const cv::Mat union_mask = redMask(hsv, true);
    std::cout << "red 0-10 nonzero=" << cv::countNonZero(low) << '\n';
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(170, 80, 60), cv::Scalar(179, 255, 255),
                high);
    std::cout << "red 170-179 nonzero=" << cv::countNonZero(high) << '\n';
    std::cout << "union nonzero=" << cv::countNonZero(union_mask) << '\n';

    cv::Mat closed;
    cv::morphologyEx(union_mask, closed, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               cv::Size(7, 7)));
    std::cout << "closed nonzero=" << cv::countNonZero(closed) << '\n';

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(closed.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point>> kept;
    for (const std::vector<cv::Point>& contour : contours) {
        if (cv::contourArea(contour) >= 1000.0) {
            kept.push_back(contour);
        }
    }
    std::cout << "contours>=1000: " << kept.size() << '\n';

    int apples = 0;
    int largest_area = 0;
    cv::Rect largest_box;
    for (const std::vector<cv::Point>& contour : kept) {
        const int area = cvRound(cv::contourArea(contour));
        if (area > largest_area) {
            largest_area = area;
            largest_box = cv::boundingRect(contour);
        }
        ++apples;
    }
    std::cout << "largest area=" << largest_area << '\n';
    std::cout << "box=(" << largest_box.x << ", " << largest_box.y << ", "
              << largest_box.width << ", " << largest_box.height << ")\n";
    std::cout << "apples=" << apples << '\n';

    cv::Mat annotated = image.clone();
    for (const std::vector<cv::Point>& contour : kept) {
        const cv::Rect box = cv::boundingRect(contour);
        cv::rectangle(annotated, box, cv::Scalar(0, 255, 0), 3);
        cv::drawContours(annotated, std::vector<std::vector<cv::Point>>{contour},
                         -1, cv::Scalar(0, 200, 255), 2);
    }
    cv::putText(annotated, "apples=" + std::to_string(apples),
                cv::Point(24, 48), cv::FONT_HERSHEY_SIMPLEX, 1.1,
                cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

    cv::Mat low_color;
    cv::cvtColor(low, low_color, cv::COLOR_GRAY2BGR);
    cv::Mat union_color;
    cv::cvtColor(closed, union_color, cv::COLOR_GRAY2BGR);

    const int cell_width = 360;
    const int cell_height = 430;
    cv::Mat panel(cell_height, 4 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, low_color, cell_width, 0, cell_width, cell_height, "H 0-10");
    place(panel, union_color, 2 * cell_width, 0, cell_width, cell_height,
          "0-10 + 170-179");
    place(panel, annotated, 3 * cell_width, 0, cell_width, cell_height,
          "annotated");
    const bool saved = cv::imwrite("out_apple.png", panel);
    std::cout << "saved out_apple.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
