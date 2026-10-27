#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

void report(const std::string& name, const std::vector<cv::Point>& contour) {
    const double area = cv::contourArea(contour);
    const double perimeter = cv::arcLength(contour, true);
    const double circularity = 4.0 * CV_PI * area / (perimeter * perimeter);
    const cv::Rect box = cv::boundingRect(contour);
    const double aspect = static_cast<double>(std::max(box.width, box.height)) /
                          std::min(box.width, box.height);
    const bool ok = area >= 500.0 && area <= 3500.0 &&
                    circularity >= 0.80 && aspect >= 1.0 && aspect <= 1.35;
    std::cout << name << " area=" << cvRound(area)
              << " circ=" << cvRound(circularity * 100.0) / 100.0
              << " aspect=" << cvRound(aspect * 100.0) / 100.0
              << " ok=" << (ok ? "yes" : "no") << '\n';
}

}  // namespace

int main() {
    cv::Mat mask = cv::Mat::zeros(60, 180, CV_8UC1);
    cv::circle(mask, cv::Point(30, 30), 20, cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(70, 10, 60, 30), cv::Scalar(255),
                  cv::FILLED);
    cv::rectangle(mask, cv::Rect(150, 5, 6, 55), cv::Scalar(255),
                  cv::FILLED);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::sort(contours.begin(), contours.end(),
              [](const std::vector<cv::Point>& a,
                 const std::vector<cv::Point>& b) {
                  return cv::boundingRect(a).x < cv::boundingRect(b).x;
              });
    for (const std::vector<cv::Point>& contour : contours) {
        const cv::Rect box = cv::boundingRect(contour);
        if (box.x < 60) {
            report("circle", contour);
        } else if (box.x < 140) {
            report("rect", contour);
        } else {
            report("bar", contour);
        }
    }
    return 0;
}
