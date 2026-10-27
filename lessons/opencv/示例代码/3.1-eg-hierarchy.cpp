#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(16, 16, CV_8UC1);
    cv::rectangle(mask, cv::Rect(2, 2, 10, 10), cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(5, 5, 4, 4), cv::Scalar(0), cv::FILLED);
    cv::rectangle(mask, cv::Rect(13, 13, 2, 2), cv::Scalar(255), cv::FILLED);

    std::vector<std::vector<cv::Point>> external;
    std::vector<std::vector<cv::Point>> tree;
    std::vector<cv::Vec4i> external_hierarchy;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask.clone(), external, external_hierarchy,
                     cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(mask.clone(), tree, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);
    std::cout << "external=" << external.size() << '\n';
    std::cout << "tree=" << tree.size() << '\n';
    for (std::size_t i = 0; i < tree.size(); ++i) {
        std::cout << "i=" << i << " child=" << hierarchy[i][2]
                  << " parent=" << hierarchy[i][3] << '\n';
    }
    return 0;
}
