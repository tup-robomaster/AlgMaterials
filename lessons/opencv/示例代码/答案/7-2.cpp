#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    const std::vector<cv::Point2f> source = {
        cv::Point2f(0.0f, 0.0f), cv::Point2f(1.0f, 0.0f),
        cv::Point2f(1.0f, 1.0f), cv::Point2f(0.0f, 1.0f),
    };
    const std::vector<cv::Point2f> target = {
        cv::Point2f(100.0f, 100.0f), cv::Point2f(400.0f, 120.0f),
        cv::Point2f(380.0f, 400.0f), cv::Point2f(120.0f, 380.0f),
    };
    const cv::Mat H = cv::getPerspectiveTransform(source, target);
    const std::vector<cv::Point2f> input = {cv::Point2f(0.25f, 0.75f)};
    std::vector<cv::Point2f> output;
    cv::perspectiveTransform(input, output, H);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "H =\n" << H << '\n';
    std::cout << "(0.25, 0.75) -> (" << output[0].x << ", "
              << output[0].y << ")\n";
    return 0;
}
