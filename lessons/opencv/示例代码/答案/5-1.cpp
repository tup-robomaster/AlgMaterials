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

    std::cout << "opened=1 width="
              << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH)) << " height="
              << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT)) << " fps="
              << std::fixed << std::setprecision(2)
              << cap.get(cv::CAP_PROP_FPS) << " frames="
              << cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT)) << '\n';

    const bool moved = cap.set(cv::CAP_PROP_POS_FRAMES, 44);
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cout << "读取失败" << '\n';
        return 1;
    }

    const cv::Vec3b center =
        frame.at<cv::Vec3b>(frame.rows / 2, frame.cols / 2);
    std::cout << "seek=" << moved
              << " pos=" << cvRound(cap.get(cv::CAP_PROP_POS_FRAMES))
              << " timestamp_ms=" << cvRound(cap.get(cv::CAP_PROP_POS_MSEC))
              << " center=(" << static_cast<int>(center[0]) << ","
              << static_cast<int>(center[1]) << ","
              << static_cast<int>(center[2]) << ")" << '\n';

    int decoded = 1;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        ++decoded;
    }
    std::cout << "decoded=" << decoded << '\n';
    return 0;
}
