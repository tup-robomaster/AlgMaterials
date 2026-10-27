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

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls_3.png", cv::IMREAD_COLOR);
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
    std::cout << "mask nonzero=" << cv::countNonZero(mask) << '\n';

    std::vector<std::vector<cv::Point>> external;
    std::vector<std::vector<cv::Point>> tree;
    std::vector<cv::Vec4i> external_hierarchy;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask.clone(), external, external_hierarchy,
                     cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(mask.clone(), tree, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);
    std::cout << "external contours=" << external.size() << '\n';
    std::cout << "tree contours=" << tree.size() << '\n';
    for (std::size_t i = 0; i < tree.size(); ++i) {
        if (hierarchy[i][3] >= 0) {
            std::cout << "nested i=" << i << " parent=" << hierarchy[i][3]
                      << " first=(" << tree[i].front().x << ", "
                      << tree[i].front().y << ")\n";
        }
    }

    cv::Mat overlay = image.clone();
    cv::drawContours(overlay, external, -1, cv::Scalar(0, 220, 0), 2);
    std::vector<std::vector<cv::Point>> holes;
    for (std::size_t i = 0; i < tree.size(); ++i) {
        if (hierarchy[i][3] >= 0) {
            holes.push_back(tree[i]);
        }
    }
    cv::drawContours(overlay, holes, -1, cv::Scalar(0, 0, 255), 2);

    cv::Mat mask_color;
    cv::cvtColor(mask, mask_color, cv::COLOR_GRAY2BGR);
    const int cell_width = 380;
    const int cell_height = 460;
    cv::Mat panel(cell_height, 3 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, mask_color, cell_width, 0, cell_width, cell_height,
          "green mask");
    place(panel, overlay, 2 * cell_width, 0, cell_width, cell_height,
          "external + holes");
    const bool saved = cv::imwrite("out_contours.png", panel);
    std::cout << "saved out_contours.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
