#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

const char* const kNames[] = {
    "synth_balls_1.png", "synth_balls_2.png",
    "synth_balls_3.png", "synth_balls_4.png",
};

struct Ball {
    cv::Point2f center;
    float radius = 0.0F;
    double area = 0.0;
    double circularity = 0.0;
};

cv::Mat greenMask(const cv::Mat& image) {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(35, 60, 50), cv::Scalar(85, 255, 255),
                mask);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(3, 3)));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(5, 5)));
    return mask;
}

double circularity(const std::vector<cv::Point>& contour) {
    const double area = cv::contourArea(contour);
    const double perimeter = cv::arcLength(contour, true);
    if (perimeter <= 0.0) {
        return 0.0;
    }
    return 4.0 * CV_PI * area / (perimeter * perimeter);
}

std::vector<Ball> findBalls(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<Ball> balls;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        const double circ = circularity(contour);
        const cv::Rect box = cv::boundingRect(contour);
        const double aspect = static_cast<double>(box.width) / box.height;
        if (area < 1500.0 || area > 9000.0 || circ < 0.75 ||
            aspect < 0.75 || aspect > 1.35) {
            continue;
        }
        Ball ball;
        cv::minEnclosingCircle(contour, ball.center, ball.radius);
        ball.area = area;
        ball.circularity = circ;
        balls.push_back(ball);
    }
    return balls;
}

void place(cv::Mat& panel, const cv::Mat& image, int x, int y, int width,
           int height, const std::string& label) {
    const int label_height = 28;
    cv::rectangle(panel, cv::Rect(x, y, width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);

    const int inner_width = width - 16;
    const int inner_height = height - label_height - 16;
    const double scale = std::min(static_cast<double>(inner_width) / image.cols,
                                  static_cast<double>(inner_height) / image.rows);
    const int shown_width = std::max(1, cvRound(image.cols * scale));
    const int shown_height = std::max(1, cvRound(image.rows * scale));
    cv::Mat shown;
    cv::resize(image, shown, cv::Size(shown_width, shown_height), 0, 0,
               cv::INTER_AREA);
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

}  // namespace

int main() {
    const int cell_width = 450;
    const int cell_height = 500;
    cv::Mat panel(2 * cell_height, 2 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    int total = 0;

    for (int i = 0; i < 4; ++i) {
        const std::string path = std::string("../素材/") + kNames[i];
        cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "无法读取 " << kNames[i] << '\n';
            return 1;
        }
        const cv::Mat mask = greenMask(image);
        const std::vector<Ball> balls = findBalls(mask);
        total += static_cast<int>(balls.size());
        std::cout << kNames[i] << ": accepted=" << balls.size() << '\n';

        cv::Mat annotated = image.clone();
        for (const Ball& ball : balls) {
            cv::circle(annotated, ball.center, cvRound(ball.radius),
                       cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
            cv::circle(annotated, ball.center, 3, cv::Scalar(0, 0, 255),
                       cv::FILLED, cv::LINE_AA);
        }
        cv::putText(annotated, "count=" + std::to_string(balls.size()),
                    cv::Point(20, 42), cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

        const int x = (i % 2) * cell_width;
        const int y = (i / 2) * cell_height;
        place(panel, annotated, x, y, cell_width, cell_height, kNames[i]);
    }
    std::cout << "total=" << total << '\n';

    const bool saved = cv::imwrite("out_green_count.png", panel);
    std::cout << "saved out_green_count.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
