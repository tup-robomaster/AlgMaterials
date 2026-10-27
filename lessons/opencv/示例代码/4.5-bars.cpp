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

int componentCount(const cv::Mat& mask) {
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    const int total = cv::connectedComponentsWithStats(
        mask, labels, stats, centroids, 8, CV_32S);
    int count = 0;
    for (int i = 1; i < total; ++i) {
        if (stats.at<int>(i, cv::CC_STAT_AREA) >= 50) {
            ++count;
        }
    }
    return count;
}

cv::Mat redMask(const cv::Mat& hsv) {
    cv::Mat low;
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), low);
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(170, 80, 80), cv::Scalar(179, 255, 255),
                high);
    cv::Mat mask;
    cv::bitwise_or(low, high, mask);
    return mask;
}

cv::Mat blueMask(const cv::Mat& hsv) {
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(100, 80, 80), cv::Scalar(130, 255, 255),
                mask);
    return mask;
}

cv::Mat colorDifference(const cv::Mat& image, bool red_minus_blue) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    cv::Mat diff;
    if (red_minus_blue) {
        cv::subtract(channels[2], channels[0], diff, cv::noArray(), CV_16S);
    } else {
        cv::subtract(channels[0], channels[2], diff, cv::noArray(), CV_16S);
    }
    cv::Mat mask;
    cv::compare(diff, 80, mask, cv::CMP_GT);
    return mask;
}

cv::Mat verticalOpen(const cv::Mat& mask) {
    cv::Mat opened;
    cv::morphologyEx(mask, opened, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(5, 25)));
    return opened;
}

cv::Mat keepBars(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    cv::Mat kept = cv::Mat::zeros(mask.size(), CV_8UC1);
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        const cv::Rect box = cv::boundingRect(contour);
        const double aspect = static_cast<double>(
            std::max(box.width, box.height)) / std::min(box.width, box.height);
        if (area >= 800.0 && aspect >= 2.0) {
            cv::drawContours(kept, std::vector<std::vector<cv::Point>>{contour},
                             -1, cv::Scalar(255), cv::FILLED);
        }
    }
    return kept;
}

int countBars(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    return static_cast<int>(contours.size());
}

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/synth_bars.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    cv::Mat gray_mask;
    cv::inRange(hsv, cv::Scalar(0, 0, 200), cv::Scalar(179, 255, 255),
                gray_mask);
    const cv::Mat red = redMask(hsv);
    const cv::Mat blue = blueMask(hsv);
    const cv::Mat red_diff = colorDifference(image, true);
    const cv::Mat blue_diff = colorDifference(image, false);

    std::cout << "gray V>=200: objects=" << componentCount(gray_mask) << '\n';
    std::cout << "R-B>80: objects=" << componentCount(red_diff) << '\n';
    std::cout << "B-R>80: objects=" << componentCount(blue_diff) << '\n';
    std::cout << "red HSV: objects=" << componentCount(red) << '\n';
    std::cout << "blue HSV: objects=" << componentCount(blue) << '\n';

    const cv::Mat red_bars_mask = keepBars(verticalOpen(red));
    const cv::Mat blue_bars_mask = keepBars(verticalOpen(blue));
    const int red_bars = countBars(red_bars_mask);
    const int blue_bars = countBars(blue_bars_mask);
    std::cout << "red bars=" << red_bars << '\n';
    std::cout << "blue bars=" << blue_bars << '\n';

    cv::Mat annotated = image.clone();
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_bars_mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    for (const std::vector<cv::Point>& contour : contours) {
        cv::rectangle(annotated, cv::boundingRect(contour),
                      cv::Scalar(0, 255, 0), 2);
    }
    cv::findContours(blue_bars_mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    for (const std::vector<cv::Point>& contour : contours) {
        cv::rectangle(annotated, cv::boundingRect(contour),
                      cv::Scalar(0, 255, 0), 2);
    }
    cv::putText(annotated, "red=" + std::to_string(red_bars) +
                                " blue=" + std::to_string(blue_bars),
                cv::Point(20, 42), cv::FONT_HERSHEY_SIMPLEX, 0.9,
                cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

    cv::Mat red_color;
    cv::cvtColor(red_bars_mask, red_color, cv::COLOR_GRAY2BGR);
    cv::Mat blue_color;
    cv::cvtColor(blue_bars_mask, blue_color, cv::COLOR_GRAY2BGR);

    const int cell_width = 500;
    const int cell_height = 400;
    cv::Mat panel(2 * cell_height, 2 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, annotated, cell_width, 0, cell_width, cell_height,
          "bars: red=3 blue=3");
    place(panel, red_color, 0, cell_height, cell_width, cell_height,
          "red bars mask");
    place(panel, blue_color, cell_width, cell_height, cell_width, cell_height,
          "blue bars mask");

    const bool saved = cv::imwrite("out_bars.png", panel);
    std::cout << "saved out_bars.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
