#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat canvas(240, 320, CV_8UC3, cv::Scalar(30, 30, 30));

    cv::rectangle(canvas, cv::Rect(20, 20, 120, 70), cv::Scalar(0, 0, 255),
                  2);
    cv::circle(canvas, cv::Point(220, 120), 40, cv::Scalar(255, 0, 0),
               cv::FILLED);
    cv::line(canvas, cv::Point(20, 180), cv::Point(300, 180),
             cv::Scalar(0, 255, 0), 3);

    int baseline = 0;
    const cv::Size text =
        cv::getTextSize("TUP", cv::FONT_HERSHEY_SIMPLEX, 0.8, 2, &baseline);
    cv::putText(canvas, "TUP", cv::Point(20, 220), cv::FONT_HERSHEY_SIMPLEX,
                0.8, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
    std::cout << "text size=" << text.width << "x" << text.height
              << " baseline=" << baseline << '\n';

    const bool saved = cv::imwrite("out_eg_draw.png", canvas);
    std::cout << "saved out_eg_draw.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
