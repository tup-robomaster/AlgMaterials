#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat image = cv::imread("../素材/synth_balls.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：../素材/synth_balls.png" << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(85, 255, 255), mask);

    const cv::Mat kernel =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> kept;
    for (const auto& contour : contours) {
        if (cv::contourArea(contour) >= 300.0) {
            kept.push_back(contour);
        }
    }

    for (std::size_t i = 0; i < kept.size(); ++i) {
        const cv::Rect box = cv::boundingRect(kept[i]);
        cv::rectangle(image, box, cv::Scalar(0, 0, 255), 2);
        cv::putText(image, std::to_string(i + 1),
                    cv::Point(box.x, box.y - 6), cv::FONT_HERSHEY_SIMPLEX,
                    0.6, cv::Scalar(0, 0, 255), 2);
    }

    std::cout << "contours=" << contours.size() << '\n';
    std::cout << "kept=" << kept.size() << '\n';

    const bool saved = cv::imwrite("out_count.png", image);
    std::cout << "saved out_count.png: " << (saved ? "yes" : "no") << '\n';

    return 0;
}
