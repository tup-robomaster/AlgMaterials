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
    cv::Mat mask = cv::Mat::zeros(10, 10, CV_8UC1);
    cv::rectangle(mask, cv::Rect(1, 3, 3, 3), cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(5, 3, 3, 3), cv::Scalar(255), cv::FILLED);

    const cv::Mat kernel =
        cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat closed;
    cv::morphologyEx(mask, closed, cv::MORPH_CLOSE, kernel);

    std::cout << "before nonzero=" << cv::countNonZero(mask)
              << " contours=" << count_contours(mask) << '\n';
    std::cout << "after nonzero=" << cv::countNonZero(closed)
              << " contours=" << count_contours(closed) << '\n';
    return 0;
}
