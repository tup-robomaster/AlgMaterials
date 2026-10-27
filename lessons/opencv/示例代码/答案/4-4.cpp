#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

int countHoles(const std::vector<cv::Vec4i>& hierarchy) {
    int holes = 0;
    for (const cv::Vec4i& item : hierarchy) {
        if (item[3] >= 0) {
            ++holes;
        }
    }
    return holes;
}

}  // namespace

int main() {
    cv::Mat mask = cv::Mat::zeros(50, 50, CV_8UC1);
    cv::circle(mask, cv::Point(25, 25), 15, cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(22, 22, 6, 6), cv::Scalar(0), cv::FILLED);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask.clone(), contours, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);
    const int before = countHoles(hierarchy);

    cv::Mat closed;
    cv::morphologyEx(mask, closed, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               cv::Size(11, 11)));
    contours.clear();
    hierarchy.clear();
    cv::findContours(closed.clone(), contours, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);
    const int after = countHoles(hierarchy);

    std::vector<std::vector<cv::Point>> outer;
    cv::findContours(closed.clone(), outer, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    std::cout << "holes before=" << before << " after=" << after
              << " objects=" << outer.size() << '\n';
    return 0;
}
