#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

struct Bar {
    int cx = 0;
    int cy = 0;
    double area = 0.0;
    double width = 0.0;
    double height = 0.0;
    double fill = 0.0;
    double angle = 0.0;
    cv::RotatedRect rect;
};

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

cv::Mat buildMask(const cv::Mat& gray) {
    cv::Mat mask;
    cv::threshold(gray, mask, 190, 255, cv::THRESH_BINARY);
    const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                                     cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    return mask;
}

std::vector<Bar> findBars(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<Bar> bars;
    for (const auto& contour : contours) {
        const double area = cv::contourArea(contour);
        if (area < 500.0 || area > 10000.0) {
            continue;
        }
        const cv::Rect box = cv::boundingRect(contour);
        if (box.width == 0 || box.height == 0) {
            continue;
        }
        const double aspect = static_cast<double>(box.height) / box.width;
        const double fill = area / (box.width * box.height);
        const double angle = axisAngle(contour);
        if (aspect < 2.5 || aspect > 12.0 || fill < 0.45 ||
            std::fabs(std::fabs(angle) - 90.0) > 30.0) {
            continue;
        }
        Bar bar;
        bar.cx = cvRound(box.x + box.width / 2.0);
        bar.cy = cvRound(box.y + box.height / 2.0);
        bar.area = area;
        bar.width = box.width;
        bar.height = box.height;
        bar.fill = fill;
        bar.angle = angle;
        bar.rect = cv::minAreaRect(contour);
        bars.push_back(bar);
    }
    std::sort(bars.begin(), bars.end(),
              [](const Bar& a, const Bar& b) { return a.cx < b.cx; });
    return bars;
}

}  // namespace

int main() {
    cv::VideoCapture cap("../素材/synth_armor.mp4");
    if (!cap.isOpened()) {
        std::cout << "无法打开 synth_armor.mp4\n";
        return 1;
    }
    const int frame_index = 40;
    cap.set(cv::CAP_PROP_POS_FRAMES, frame_index);
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cout << "无法读取第 " << frame_index << " 帧\n";
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    const cv::Mat mask = buildMask(gray);
    const std::vector<Bar> bars = findBars(mask);

    cv::Mat annotated = frame.clone();
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "frame = " << frame_index << '\n';
    std::cout << "bars = " << bars.size() << '\n';
    std::cout << " idx     cx     cy     area   width  height   fill  angle\n";
    for (std::size_t i = 0; i < bars.size(); ++i) {
        const Bar& bar = bars[i];
        cv::Point2f corners[4];
        bar.rect.points(corners);
        for (int k = 0; k < 4; ++k) {
            cv::line(annotated, cv::Point(cvRound(corners[k].x),
                                          cvRound(corners[k].y)),
                     cv::Point(cvRound(corners[(k + 1) % 4].x),
                               cvRound(corners[(k + 1) % 4].y)),
                     cv::Scalar(40, 220, 40), 2, cv::LINE_AA);
        }
        cv::putText(annotated, std::to_string(i),
                    cv::Point(bar.cx + 8, bar.cy - 8),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(40, 220, 40),
                    2, cv::LINE_AA);
        std::cout << std::setw(4) << i << std::setw(7) << bar.cx
                  << std::setw(7) << bar.cy << std::setw(9) << bar.area
                  << std::setw(8) << bar.width << std::setw(8) << bar.height
                  << std::setw(7) << bar.fill << std::setw(7) << bar.angle
                  << '\n';
    }

    cv::Mat mask_bgr;
    cv::cvtColor(mask, mask_bgr, cv::COLOR_GRAY2BGR);
    cv::Mat panel;
    cv::hconcat(annotated, mask_bgr, panel);
    cv::putText(panel, "light bar candidates", cv::Point(16, 32),
                cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(40, 220, 40), 2,
                cv::LINE_AA);
    cv::putText(panel, "threshold mask", cv::Point(656, 32),
                cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(40, 220, 40), 2,
                cv::LINE_AA);
    const bool saved = cv::imwrite("../素材/out_armor_bars.png", panel);
    std::cout << "saved out_armor_bars.png " << panel.cols << "x" << panel.rows
              << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
