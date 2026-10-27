#include <cmath>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

double axisAngle(const std::vector<cv::Point>& contour) {
    cv::Mat samples(static_cast<int>(contour.size()), 2, CV_64F);
    for (int i = 0; i < samples.rows; ++i) {
        samples.at<double>(i, 0) = contour[i].x;
        samples.at<double>(i, 1) = contour[i].y;
    }
    cv::PCA pca(samples, cv::Mat(), cv::PCA::DATA_AS_ROW);
    const double ax = pca.eigenvectors.at<double>(0, 0);
    const double ay = pca.eigenvectors.at<double>(0, 1);
    double angle = std::atan2(ay, ax) * 180.0 / CV_PI;
    if (angle > 90.0) {
        angle -= 180.0;
    }
    if (angle <= -90.0) {
        angle += 180.0;
    }
    return angle;
}

int countBars(const cv::Mat& frame) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Mat mask;
    cv::threshold(gray, mask, 190, 255, cv::THRESH_BINARY);
    const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                                     cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    int count = 0;
    for (const auto& contour : contours) {
        const double area = cv::contourArea(contour);
        const cv::Rect box = cv::boundingRect(contour);
        if (box.width == 0 || box.height == 0) {
            continue;
        }
        const double aspect = static_cast<double>(box.height) / box.width;
        const double fill = area / (box.width * box.height);
        const double angle = axisAngle(contour);
        if (area >= 500.0 && area <= 10000.0 && aspect >= 2.5 &&
            aspect <= 12.0 && fill >= 0.45 &&
            std::fabs(std::fabs(angle) - 90.0) <= 30.0) {
            ++count;
        }
    }
    return count;
}

cv::Mat readFrame(cv::VideoCapture& cap, int index) {
    cap.set(cv::CAP_PROP_POS_FRAMES, index);
    cv::Mat frame;
    cap >> frame;
    return frame;
}

}  // namespace

int main() {
    cv::VideoCapture cap("../素材/synth_armor.mp4");
    if (!cap.isOpened()) {
        std::cout << "open failed\n";
        return 1;
    }
    const cv::Mat first = readFrame(cap, 0);
    const cv::Mat gap = readFrame(cap, 150);
    std::cout << "frame_0_bars = " << countBars(first) << '\n';
    std::cout << "frame_150_bars = " << countBars(gap) << '\n';
    return 0;
}
