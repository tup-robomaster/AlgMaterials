#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

int main() {
    cv::VideoCapture cap("../素材/cube.mp4", cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cout << "无法打开视频" << '\n';
        return 1;
    }

    int frames = 0;
    int brightest = 0;
    double max_mean = -1.0;
    int last_ms = 0;

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        ++frames;
        last_ms = cvRound(cap.get(cv::CAP_PROP_POS_MSEC));
        const double mean = cv::mean(frame)[0];
        if (mean > max_mean) {
            max_mean = mean;
            brightest = frames;
        }
    }

    std::cout << "frames=" << frames << " first_ms=0 last_ms=" << last_ms
              << " brightest=" << brightest << " max_mean="
              << std::fixed << std::setprecision(1) << max_mean << '\n';
    return 0;
}
