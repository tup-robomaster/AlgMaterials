#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat hsv(20, 20, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::rectangle(hsv, cv::Rect(2, 2, 5, 4), cv::Scalar(175, 255, 255),
                  cv::FILLED);

    cv::Mat reversed;
    cv::inRange(hsv, cv::Scalar(179, 80, 80), cv::Scalar(170, 255, 255),
                reversed);
    cv::Mat correct;
    cv::inRange(hsv, cv::Scalar(170, 80, 80), cv::Scalar(179, 255, 255),
                correct);
    cv::Mat low;
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), low);

    std::cout << "reversed 179..170: nonzero=" << cv::countNonZero(reversed)
              << '\n';
    std::cout << "correct 170..179: nonzero=" << cv::countNonZero(correct)
              << '\n';
    std::cout << "low 0..10: nonzero=" << cv::countNonZero(low) << '\n';
    return 0;
}
