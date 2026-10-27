#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(18, 18, CV_8UC1);
    cv::rectangle(mask, cv::Rect(2, 2, 8, 8), cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(4, 4, 4, 4), cv::Scalar(0), cv::FILLED);
    cv::rectangle(mask, cv::Rect(13, 2, 4, 4), cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(2, 13, 4, 4), cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(12, 12, 4, 4), cv::Scalar(255), cv::FILLED);

    std::vector<std::vector<cv::Point>> external;
    std::vector<std::vector<cv::Point>> tree;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask.clone(), external, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(mask.clone(), tree, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);

    int nested = 0;
    for (const cv::Vec4i& item : hierarchy) {
        if (item[3] >= 0) {
            ++nested;
        }
    }
    std::cout << "external=" << external.size() << '\n';
    std::cout << "tree=" << tree.size() << '\n';
    std::cout << "nested=" << nested << '\n';
    return 0;
}
