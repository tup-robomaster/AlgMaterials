#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

int count_contours(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    return static_cast<int>(contours.size());
}

}  // namespace

int main() {
    cv::Mat mask = cv::Mat::zeros(9, 9, CV_8UC1);
    cv::rectangle(mask, cv::Rect(2, 2, 4, 4), cv::Scalar(255), cv::FILLED);
    mask.at<uchar>(7, 7) = 255;

    const cv::Mat kernel =
        cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat eroded;
    cv::Mat opened;
    cv::erode(mask, eroded, kernel);
    cv::morphologyEx(mask, opened, cv::MORPH_OPEN, kernel);

    std::cout << "input nonzero=" << cv::countNonZero(mask)
              << " contours=" << count_contours(mask) << '\n';
    std::cout << "erode nonzero=" << cv::countNonZero(eroded)
              << " contours=" << count_contours(eroded) << '\n';
    std::cout << "open nonzero=" << cv::countNonZero(opened)
              << " contours=" << count_contours(opened) << '\n';
    return 0;
}
