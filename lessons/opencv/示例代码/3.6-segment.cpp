#include <algorithm>
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
    cv::Mat image = cv::imread("../素材/synth_touching.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::Mat mask;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(85, 255, 255), mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(3, 3)));
    const int foreground = cv::countNonZero(mask);
    std::cout << "mask nonzero=" << foreground << '\n';

    cv::Mat distance;
    cv::distanceTransform(mask, distance, cv::DIST_L2, 5);
    double min_distance = 0.0;
    double max_distance = 0.0;
    cv::minMaxLoc(distance, &min_distance, &max_distance);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "distance max=" << max_distance << '\n';

    cv::Mat seeds;
    cv::threshold(distance, seeds, 0.55 * max_distance, 255.0,
                  cv::THRESH_BINARY);
    seeds.convertTo(seeds, CV_8U);
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    const int seed_count = cv::connectedComponentsWithStats(
        seeds, labels, stats, centroids, 8) - 1;
    std::cout << "seeds=" << seed_count << '\n';

    cv::Mat markers(mask.size(), CV_32S, cv::Scalar(0));
    for (int y = 0; y < labels.rows; ++y) {
        for (int x = 0; x < labels.cols; ++x) {
            if (labels.at<int>(y, x) > 0) {
                markers.at<int>(y, x) = labels.at<int>(y, x);
            }
        }
    }
    markers.setTo(seed_count + 1, mask == 0);
    cv::watershed(image, markers);
    double marker_min = 0.0;
    double marker_max = 0.0;
    cv::minMaxLoc(markers, &marker_min, &marker_max);
    std::cout << "watershed labels=" << static_cast<int>(marker_max) << '\n';

    std::vector<bool> present(static_cast<std::size_t>(marker_max) + 1, false);
    for (int y = 0; y < markers.rows; ++y) {
        for (int x = 0; x < markers.cols; ++x) {
            const int value = markers.at<int>(y, x);
            if (value > 0 && value <= seed_count) {
                present[static_cast<std::size_t>(value)] = true;
            }
        }
    }
    int ball_regions = 0;
    for (int i = 1; i <= seed_count; ++i) {
        if (present[static_cast<std::size_t>(i)]) {
            ++ball_regions;
        }
    }
    std::cout << "ball regions=" << ball_regions << '\n';

    cv::Mat background = mask.clone();
    const int flood_area = cv::floodFill(background, cv::Point(0, 0),
                                         cv::Scalar(128));
    std::cout << "flood background=" << flood_area << '\n';

    cv::Mat distance_view;
    cv::normalize(distance, distance_view, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::applyColorMap(distance_view, distance_view, cv::COLORMAP_JET);

    const cv::Scalar palette[12] = {
        cv::Scalar(60, 60, 220), cv::Scalar(60, 180, 240),
        cv::Scalar(60, 200, 60), cv::Scalar(220, 180, 60),
        cv::Scalar(220, 80, 200), cv::Scalar(180, 120, 60),
        cv::Scalar(200, 220, 60), cv::Scalar(60, 220, 200),
        cv::Scalar(240, 140, 140), cv::Scalar(140, 240, 140),
        cv::Scalar(140, 140, 240), cv::Scalar(180, 180, 180)};
    cv::Mat segments(mask.size(), CV_8UC3, cv::Scalar(245, 245, 245));
    for (int y = 0; y < markers.rows; ++y) {
        for (int x = 0; x < markers.cols; ++x) {
            const int value = markers.at<int>(y, x);
            if (value >= 1 && value <= seed_count) {
                segments.at<cv::Vec3b>(y, x) = cv::Vec3b(
                    palette[(value - 1) % 12][0], palette[(value - 1) % 12][1],
                    palette[(value - 1) % 12][2]);
            }
        }
    }

    cv::Mat mask_color;
    cv::cvtColor(mask, mask_color, cv::COLOR_GRAY2BGR);

    const int cell_width = 320;
    const int cell_height = 360;
    cv::Mat panel(cell_height, 4 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, mask_color, cell_width, 0, cell_width, cell_height, "mask");
    place(panel, distance_view, 2 * cell_width, 0, cell_width, cell_height,
          "distance");
    place(panel, segments, 3 * cell_width, 0, cell_width, cell_height,
          "watershed");
    const bool saved = cv::imwrite("out_segment.png", panel);
    std::cout << "saved out_segment.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
