#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

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
               cv::INTER_NEAREST);
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

cv::Mat makeScene() {
    cv::Mat scene(240, 240, CV_8UC3);
    scene(cv::Rect(0, 0, 120, 120)).setTo(cv::Scalar(30, 30, 210));
    scene(cv::Rect(120, 0, 120, 120)).setTo(cv::Scalar(220, 80, 30));
    scene(cv::Rect(0, 120, 120, 120)).setTo(cv::Scalar(30, 190, 60));
    scene(cv::Rect(120, 120, 120, 120)).setTo(cv::Scalar(230, 230, 230));

    cv::circle(scene, cv::Point(60, 60), 30, cv::Scalar(235, 235, 235),
               cv::FILLED, cv::LINE_AA);
    cv::circle(scene, cv::Point(180, 60), 30, cv::Scalar(20, 20, 20),
               cv::FILLED, cv::LINE_AA);
    cv::circle(scene, cv::Point(60, 180), 30, cv::Scalar(15, 15, 15),
               cv::FILLED, cv::LINE_AA);
    cv::line(scene, cv::Point(130, 230), cv::Point(230, 130),
             cv::Scalar(30, 30, 30), 3, cv::LINE_AA);
    return scene;
}

cv::Mat mosaicRG(const cv::Mat& bgr) {
    cv::Mat bayer(bgr.rows, bgr.cols, CV_8UC1);
    for (int y = 0; y < bgr.rows; ++y) {
        for (int x = 0; x < bgr.cols; ++x) {
            const cv::Vec3b pixel = bgr.at<cv::Vec3b>(y, x);
            uchar value = 0;
            if (y % 2 == 0 && x % 2 == 0) {
                value = pixel[2];
            } else if (y % 2 == 1 && x % 2 == 1) {
                value = pixel[0];
            } else {
                value = pixel[1];
            }
            bayer.at<uchar>(y, x) = value;
        }
    }
    return bayer;
}

}  // namespace

int main() {
    const int rows = 2;
    const int cols = 3;
    std::vector<uchar> buffer(rows * cols * 3, 0);
    cv::Mat wrapped(rows, cols, CV_8UC3, buffer.data(), cols * 3);
    wrapped.at<cv::Vec3b>(0, 0) = cv::Vec3b(10, 20, 30);
    wrapped.at<cv::Vec3b>(rows - 1, cols - 1) = cv::Vec3b(40, 50, 60);

    std::cout << "wrapped type=" << wrapped.type()
              << " channels=" << wrapped.channels() << " rows=" << rows
              << " cols=" << cols << " step=" << wrapped.step[0] << '\n';
    std::cout << "buffer first=(" << static_cast<int>(buffer[0]) << ","
              << static_cast<int>(buffer[1]) << ","
              << static_cast<int>(buffer[2]) << ")"
              << " last=(" << static_cast<int>(buffer[buffer.size() - 3])
              << "," << static_cast<int>(buffer[buffer.size() - 2]) << ","
              << static_cast<int>(buffer[buffer.size() - 1]) << ")" << '\n';

    const cv::Mat scene = makeScene();
    const cv::Mat bayer = mosaicRG(scene);
    cv::Mat restored;
    cv::cvtColor(bayer, restored, cv::COLOR_BayerBG2BGR);
    cv::Mat mosaic_view;
    cv::cvtColor(bayer, mosaic_view, cv::COLOR_GRAY2BGR);

    const cv::Vec3b original_pixel = scene.at<cv::Vec3b>(30, 30);
    const cv::Vec3b restored_pixel = restored.at<cv::Vec3b>(30, 30);
    cv::Mat diff;
    cv::absdiff(scene, restored, diff);
    const cv::Scalar mean_diff = cv::mean(diff);

    std::cout << "bayer type=" << bayer.type()
              << " channels=" << bayer.channels() << " rows=" << bayer.rows
              << " cols=" << bayer.cols << " step=" << bayer.step[0] << '\n';
    std::cout << "pixel original=(" << static_cast<int>(original_pixel[0])
              << "," << static_cast<int>(original_pixel[1]) << ","
              << static_cast<int>(original_pixel[2]) << ")"
              << " restored=(" << static_cast<int>(restored_pixel[0]) << ","
              << static_cast<int>(restored_pixel[1]) << ","
              << static_cast<int>(restored_pixel[2]) << ")" << '\n';
    std::cout << "mean_abs_diff=" << cvRound(mean_diff[0]) << '\n';

    cv::Mat panel(300, 780, CV_8UC3, cv::Scalar(255, 255, 255));
    place(panel, scene, 0, 0, 260, 300, "BGR scene");
    place(panel, mosaic_view, 260, 0, 260, 300, "Bayer RG raw");
    place(panel, restored, 520, 0, 260, 300, "demosaiced BGR");
    const bool saved = cv::imwrite("../素材/out_bayer.png", panel);
    std::cout << "saved out_bayer.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
