#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

int main() {
    cv::VideoCapture cap("../素材/cube.mp4", cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cout << "无法打开视频" << '\n';
        return 1;
    }

    const bool set_width = cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
    const bool set_height = cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
    const bool set_fps = cap.set(cv::CAP_PROP_FPS, 15);
    std::cout << "set=" << set_width << "/" << set_height << "/" << set_fps
              << " actual=" << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH))
              << "x" << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT))
              << " fps=" << std::fixed << std::setprecision(2)
              << cap.get(cv::CAP_PROP_FPS) << '\n';

    cap.set(cv::CAP_PROP_POS_FRAMES, 20);
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cout << "读取失败" << '\n';
        return 1;
    }

    cv::Mat mirrored;
    cv::flip(frame, mirrored, 1);
    const cv::Vec3b left = frame.at<cv::Vec3b>(100, 100);
    const cv::Vec3b right = mirrored.at<cv::Vec3b>(100, 539);
    std::cout << "pos=" << cvRound(cap.get(cv::CAP_PROP_POS_FRAMES))
              << " size=" << frame.cols << "x" << frame.rows << '\n';
    std::cout << "original(100,100)=(" << static_cast<int>(left[0]) << ","
              << static_cast<int>(left[1]) << ","
              << static_cast<int>(left[2]) << ")"
              << " mirrored(100,539)=(" << static_cast<int>(right[0]) << ","
              << static_cast<int>(right[1]) << ","
              << static_cast<int>(right[2]) << ")" << '\n';

    const bool saved = cv::imwrite("out_ex_5_2.png", mirrored);
    std::cout << "saved out_ex_5_2.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
