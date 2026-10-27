#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(50, 100, CV_8UC1);
    cv::circle(mask, cv::Point(35, 25), 15, cv::Scalar(255), cv::FILLED);
    cv::circle(mask, cv::Point(60, 25), 15, cv::Scalar(255), cv::FILLED);

    std::vector<std::vector<cv::Point>> before;
    cv::findContours(mask.clone(), before, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::cout << "before=" << before.size() << '\n';

    cv::Mat eroded;
    cv::erode(mask, eroded, cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                      cv::Size(5, 5)),
              cv::Point(-1, -1), 5);
    std::vector<std::vector<cv::Point>> after;
    cv::findContours(eroded.clone(), after, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::cout << "after=" << after.size() << '\n';

    for (std::size_t i = 0; i < after.size(); ++i) {
        std::cout << "area[" << i << "]="
                  << cvRound(cv::contourArea(after[i])) << '\n';
    }
    return 0;
}
