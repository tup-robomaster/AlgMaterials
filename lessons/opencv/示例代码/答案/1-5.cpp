#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat canvas(200, 300, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::rectangle(canvas, cv::Rect(10, 10, 100, 50), cv::Scalar(0, 0, 255),
                  2);
    cv::circle(canvas, cv::Point(200, 80), 30, cv::Scalar(0, 255, 0),
               cv::FILLED);

    int baseline = 0;
    const cv::Size text =
        cv::getTextSize("OK", cv::FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseline);
    cv::putText(canvas, "OK", cv::Point(10, 120), cv::FONT_HERSHEY_SIMPLEX,
                0.7, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    std::cout << "text size=" << text.width << "x" << text.height
              << " baseline=" << baseline << '\n';

    const bool saved = cv::imwrite("out_ex_1_5.png", canvas);
    std::cout << "saved out_ex_1_5.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
