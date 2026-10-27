#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

cv::Mat mosaicRG(const cv::Mat& bgr) {
    cv::Mat bayer(bgr.rows, bgr.cols, CV_8UC1);
    for (int y = 0; y < bgr.rows; ++y) {
        for (int x = 0; x < bgr.cols; ++x) {
            const cv::Vec3b pixel = bgr.at<cv::Vec3b>(y, x);
            if (y % 2 == 0 && x % 2 == 0) {
                bayer.at<uchar>(y, x) = pixel[2];
            } else if (y % 2 == 1 && x % 2 == 1) {
                bayer.at<uchar>(y, x) = pixel[0];
            } else {
                bayer.at<uchar>(y, x) = pixel[1];
            }
        }
    }
    return bayer;
}

void report(const std::string& name, const cv::Mat& bayer, int code) {
    cv::Mat bgr;
    cv::cvtColor(bayer, bgr, code);
    const cv::Vec3b pixel = bgr.at<cv::Vec3b>(4, 4);
    std::cout << name << " type=" << bgr.type()
              << " channels=" << bgr.channels() << " pixel=("
              << static_cast<int>(pixel[0]) << ","
              << static_cast<int>(pixel[1]) << ","
              << static_cast<int>(pixel[2]) << ")" << '\n';
}

}  // namespace

int main() {
    cv::Mat scene(16, 16, CV_8UC3);
    scene(cv::Rect(0, 0, 8, 8)).setTo(cv::Scalar(30, 30, 210));
    scene(cv::Rect(8, 0, 8, 8)).setTo(cv::Scalar(210, 80, 30));
    scene(cv::Rect(0, 8, 8, 8)).setTo(cv::Scalar(30, 190, 60));
    scene(cv::Rect(8, 8, 8, 8)).setTo(cv::Scalar(230, 230, 230));

    const cv::Mat bayer = mosaicRG(scene);
    const cv::Vec3b source = scene.at<cv::Vec3b>(4, 4);
    std::cout << "source pixel=(" << static_cast<int>(source[0]) << ","
              << static_cast<int>(source[1]) << ","
              << static_cast<int>(source[2]) << ")" << '\n';

    report("BayerBG2BGR", bayer, cv::COLOR_BayerBG2BGR);
    report("BayerRG2BGR", bayer, cv::COLOR_BayerRG2BGR);
    return 0;
}
