#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

int main() {
    cv::VideoCapture cap("../素材/cube.mp4", cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cout << "无法打开视频" << '\n';
        return 1;
    }

    const std::vector<int> targets = {0, 15, 30, 45, 60, 75};
    for (const int target : targets) {
        const bool moved = cap.set(cv::CAP_PROP_POS_FRAMES, target);
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            std::cout << "seek=" << target << " empty=1" << '\n';
            continue;
        }
        const cv::Scalar mean = cv::mean(frame);
        std::cout << "seek=" << target
                  << " pos=" << cvRound(cap.get(cv::CAP_PROP_POS_FRAMES))
                  << " timestamp_ms="
                  << cvRound(cap.get(cv::CAP_PROP_POS_MSEC))
                  << " mean_bgr=(" << cvRound(mean[0]) << ","
                  << cvRound(mean[1]) << "," << cvRound(mean[2]) << ")"
                  << " moved=" << moved << '\n';
    }
    return 0;
}
