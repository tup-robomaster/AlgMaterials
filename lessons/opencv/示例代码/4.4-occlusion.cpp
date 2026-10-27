#include <algorithm>
#include <cmath>
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

double circularity(const std::vector<cv::Point>& contour) {
    const double area = cv::contourArea(contour);
    const double perimeter = cv::arcLength(contour, true);
    if (perimeter <= 0.0) {
        return 0.0;
    }
    return 4.0 * CV_PI * area / (perimeter * perimeter);
}

cv::Mat greenMask(const cv::Mat& image) {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(35, 60, 50), cv::Scalar(85, 255, 255),
                mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(3, 3)));
    return mask;
}

int countNested(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask.clone(), contours, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);
    int nested = 0;
    for (const cv::Vec4i& item : hierarchy) {
        if (item[3] >= 0) {
            ++nested;
        }
    }
    return nested;
}

int countSingleBalls(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    int count = 0;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        const cv::Rect box = cv::boundingRect(contour);
        const double aspect = static_cast<double>(
            std::max(box.width, box.height)) / std::min(box.width, box.height);
        if (area >= 1500.0 && area <= 9000.0 && circularity(contour) >= 0.75 &&
            aspect >= 1.0 && aspect <= 1.35) {
            ++count;
        }
    }
    return count;
}

}  // namespace

int main() {
    const int cell_width = 380;
    const int cell_height = 420;
    cv::Mat panel(2 * cell_height, 3 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));

    // 白球：低 S、高 V；背景不是白色时才可用颜色单独分开。
    cv::Mat image2 = cv::imread("../素材/synth_balls_2.png", cv::IMREAD_COLOR);
    if (image2.empty()) {
        std::cout << "无法读取 synth_balls_2.png" << '\n';
        return 1;
    }
    const cv::Mat green2 = greenMask(image2);
    cv::Mat closed2;
    cv::morphologyEx(green2, closed2, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(5, 5)));
    const int green_count2 = countSingleBalls(closed2);

    cv::Mat hsv2;
    cv::cvtColor(image2, hsv2, cv::COLOR_BGR2HSV);
    cv::Mat white;
    cv::inRange(hsv2, cv::Scalar(0, 0, 215), cv::Scalar(179, 45, 255),
                white);
    std::vector<std::vector<cv::Point>> white_contours;
    cv::findContours(white.clone(), white_contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    int white_balls = 0;
    for (const std::vector<cv::Point>& contour : white_contours) {
        const double area = cv::contourArea(contour);
        const cv::Rect box = cv::boundingRect(contour);
        if (area >= 2500.0 && area <= 10000.0 &&
            circularity(contour) >= 0.75 && box.x > 1 && box.y > 1 &&
            box.br().x < white.cols - 1 && box.br().y < white.rows - 1) {
            ++white_balls;
        }
    }
    std::cout << "synth_balls_2: green=" << green_count2
              << " white=" << white_balls << '\n';

    cv::Mat white_color;
    cv::cvtColor(white, white_color, cv::COLOR_GRAY2BGR);
    cv::Mat annotated2 = image2.clone();
    cv::putText(annotated2, "green=" + std::to_string(green_count2) +
                                " white=" + std::to_string(white_balls),
                cv::Point(20, 42), cv::FONT_HERSHEY_SIMPLEX, 0.8,
                cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
    place(panel, annotated2, 0, 0, cell_width, cell_height, "balls_2");
    place(panel, white_color, cell_width, 0, cell_width, cell_height,
          "white mask");

    // 高光：先数孔洞，再用 13×13 闭运算填平。
    cv::Mat image3 = cv::imread("../素材/synth_balls_3.png", cv::IMREAD_COLOR);
    if (image3.empty()) {
        std::cout << "无法读取 synth_balls_3.png" << '\n';
        return 1;
    }
    const cv::Mat open3 = greenMask(image3);
    const int holes_before = countNested(open3);
    cv::Mat closed3;
    cv::morphologyEx(open3, closed3, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(13, 13)));
    const int holes_after = countNested(closed3);
    const int green_count3 = countSingleBalls(closed3);
    std::cout << "synth_balls_3: holes before=" << holes_before
              << " after=" << holes_after << " green=" << green_count3
              << '\n';

    cv::Mat open3_color;
    cv::cvtColor(open3, open3_color, cv::COLOR_GRAY2BGR);
    cv::Mat closed3_color;
    cv::cvtColor(closed3, closed3_color, cv::COLOR_GRAY2BGR);
    place(panel, open3_color, 2 * cell_width, 0, cell_width, cell_height,
          "holes before=7");
    place(panel, closed3_color, 0, cell_height, cell_width, cell_height,
          "closed mask, holes=0");

    // 粘连：面积约为单球的整数倍，按中位面积估计个数。
    cv::Mat image4 = cv::imread("../素材/synth_balls_4.png", cv::IMREAD_COLOR);
    if (image4.empty()) {
        std::cout << "无法读取 synth_balls_4.png" << '\n';
        return 1;
    }
    const cv::Mat open4 = greenMask(image4);
    cv::Mat closed4;
    cv::morphologyEx(open4, closed4, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(5, 5)));
    std::vector<std::vector<cv::Point>> contours4;
    cv::findContours(closed4.clone(), contours4, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    std::vector<double> single_areas;
    for (const std::vector<cv::Point>& contour : contours4) {
        if (circularity(contour) >= 0.75) {
            single_areas.push_back(cv::contourArea(contour));
        }
    }
    std::sort(single_areas.begin(), single_areas.end());
    double median_area = 0.0;
    if (!single_areas.empty()) {
        median_area = single_areas[single_areas.size() / 2];
    }
    if (median_area <= 0.0) {
        std::cout << "没有可用的单球轮廓" << '\n';
        return 1;
    }

    int singles = 0;
    int pairs = 0;
    int green_total = 0;
    cv::Mat annotated4 = image4.clone();
    for (const std::vector<cv::Point>& contour : contours4) {
        const double area = cv::contourArea(contour);
        const cv::Rect box = cv::boundingRect(contour);
        const double aspect = static_cast<double>(
            std::max(box.width, box.height)) / std::min(box.width, box.height);
        if (area > 1.5 * median_area && aspect >= 1.5) {
            const int inside = std::max(2, cvRound(area / median_area));
            pairs += 1;
            green_total += inside;
            cv::rectangle(annotated4, box, cv::Scalar(0, 255, 0), 2);
            cv::putText(annotated4, std::to_string(inside),
                        cv::Point(box.x, box.y - 6),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7,
                        cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        } else if (area >= 1500.0 && area <= 9000.0 &&
                   circularity(contour) >= 0.75 && aspect <= 1.35) {
            ++singles;
            green_total += 1;
        }
    }
    std::cout << "synth_balls_4: singles=" << singles << " pairs=" << pairs
              << " green_total=" << green_total << '\n';

    cv::putText(annotated4, "singles=" + std::to_string(singles) +
                                " pairs=" + std::to_string(pairs) +
                                " total=" + std::to_string(green_total),
                cv::Point(20, 42), cv::FONT_HERSHEY_SIMPLEX, 0.8,
                cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
    place(panel, image4, cell_width, cell_height, cell_width, cell_height,
          "balls_4");
    place(panel, annotated4, 2 * cell_width, cell_height, cell_width,
          cell_height, "touching pairs");

    const bool saved = cv::imwrite("out_occlusion.png", panel);
    std::cout << "saved out_occlusion.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
