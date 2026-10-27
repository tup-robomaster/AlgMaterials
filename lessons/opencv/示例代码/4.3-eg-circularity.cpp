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
    const double aspect = static_cast<double>(box.width) / box.height;
    std::cout << name << ": area=" << cvRound(area)
              << " perimeter=" << cvRound(perimeter)
              << " circularity=" << cvRound(circularity * 100.0) / 100.0
              << " aspect=" << cvRound(aspect * 100.0) / 100.0 << '\n';
}

}  // namespace

int main() {
    cv::Mat mask = cv::Mat::zeros(100, 200, CV_8UC1);
    cv::circle(mask, cv::Point(50, 50), 30, cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(120, 20, 60, 60), cv::Scalar(255),
                  cv::FILLED);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    for (const std::vector<cv::Point>& contour : contours) {
        const cv::Rect box = cv::boundingRect(contour);
        if (box.x < 100) {
            report("circle", contour);
        } else {
            report("square", contour);
        }
    }
    return 0;
}
