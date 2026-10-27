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

double iou(const cv::Rect& a, const cv::Rect& b) {
    const int left = std::max(a.x, b.x);
    const int top = std::max(a.y, b.y);
    const int right = std::min(a.x + a.width, b.x + b.width);
    const int bottom = std::min(a.y + a.height, b.y + b.height);
    const int overlap = std::max(0, right - left) * std::max(0, bottom - top);
    const int area = a.area() + b.area() - overlap;
    return area > 0 ? static_cast<double>(overlap) / area : 0.0;
}

std::vector<cv::Rect> candidates(const cv::Mat& image, int low_h,
                                 int low_s, int low_v) {
    cv::Mat hsv;
    cv::Mat mask;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(low_h, low_s, low_v),
                cv::Scalar(90, 255, 255), mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(3, 3)));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Rect> found;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        const double perimeter = cv::arcLength(contour, true);
        if (perimeter <= 0.0) {
            continue;
        }
        const double circularity =
            4.0 * CV_PI * area / (perimeter * perimeter);
        const cv::Rect box = cv::boundingRect(contour);
        const double aspect =
            static_cast<double>(box.width) / std::max(1, box.height);
        if (area >= 900.0 && area <= 9000.0 && aspect >= 0.70 &&
            aspect <= 1.40 && circularity >= 0.68) {
            found.push_back(box);
        }
    }
    return found;
}

std::vector<cv::Rect> deduplicate(std::vector<cv::Rect> boxes) {
    std::sort(boxes.begin(), boxes.end(), [](const cv::Rect& a,
                                             const cv::Rect& b) {
        return a.area() > b.area();
    });
    std::vector<cv::Rect> kept;
    for (const cv::Rect& box : boxes) {
        bool duplicate = false;
        for (const cv::Rect& other : kept) {
            if (iou(box, other) > 0.40) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            kept.push_back(box);
        }
    }
    return kept;
}

void annotate(cv::Mat& panel, const cv::Mat& image, int x, int y, int width,
              int height, const std::string& label,
              const std::vector<cv::Rect>& boxes) {
    const int label_height = 28;
    const int inner_width = width - 16;
    const int inner_height = height - label_height - 16;
    const double scale = std::min(static_cast<double>(inner_width) / image.cols,
                                  static_cast<double>(inner_height) / image.rows);
    const int shown_width = std::max(1, cvRound(image.cols * scale));
    const int shown_height = std::max(1, cvRound(image.rows * scale));
    cv::Mat shown;
    cv::resize(image, shown, cv::Size(shown_width, shown_height), 0, 0,
               cv::INTER_AREA);
    cv::rectangle(panel, cv::Rect(x, y, width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);
    for (std::size_t i = 0; i < boxes.size(); ++i) {
        const cv::Rect scaled(cvRound(boxes[i].x * scale),
                              cvRound(boxes[i].y * scale),
                              std::max(1, cvRound(boxes[i].width * scale)),
                              std::max(1, cvRound(boxes[i].height * scale)));
        cv::rectangle(shown, scaled, cv::Scalar(0, 0, 255), 2);
        cv::putText(shown, std::to_string(i + 1),
                    cv::Point(scaled.x + 2, scaled.y + 14),
                    cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(255, 0, 0), 1,
                    cv::LINE_AA);
    }
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

}  // namespace

int main() {
    const std::string names[4] = {
        "synth_balls_1.png", "synth_balls_2.png",
        "synth_balls_3.png", "synth_balls_4.png"};
    cv::Mat panel(2 * 500, 2 * 450, CV_8UC3, cv::Scalar(255, 255, 255));
    for (int k = 0; k < 4; ++k) {
        cv::Mat image = cv::imread("../素材/" + names[k], cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "无法读取素材: " << names[k] << '\n';
            return 1;
        }
        const std::vector<cv::Rect> loose = candidates(image, 30, 35, 35);
        const std::vector<cv::Rect> tight = candidates(image, 45, 120, 110);
        std::vector<cv::Rect> merged = loose;
        merged.insert(merged.end(), tight.begin(), tight.end());
        const std::vector<cv::Rect> kept = deduplicate(merged);
        std::cout << names[k] << ": loose=" << loose.size()
                  << " tight=" << tight.size() << " raw=" << merged.size()
                  << " kept=" << kept.size() << '\n';
        annotate(panel, image, (k % 2) * 450, (k / 2) * 500, 450, 500,
                 names[k] + " kept=" + std::to_string(kept.size()), kept);
    }
    const bool saved = cv::imwrite("out_filter.png", panel);
    std::cout << "saved out_filter.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
