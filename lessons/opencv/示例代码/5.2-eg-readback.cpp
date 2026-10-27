#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

int main() {
    cv::VideoCapture cap("../素材/cube.mp4", cv::CAP_ANY);
    std::cout << "opened=" << (cap.isOpened() ? 1 : 0) << '\n';
    if (!cap.isOpened()) {
        return 1;
    }

    const bool set_width = cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    const bool set_height = cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    const bool set_fps = cap.set(cv::CAP_PROP_FPS, 30);

    std::cout << "set width=" << set_width << " height=" << set_height
              << " fps=" << set_fps << '\n';
    std::cout << "requested=1280x720 fps=30.00" << '\n';
    std::cout << "actual=" << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH))
              << "x" << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT))
              << " fps=" << std::fixed << std::setprecision(2)
              << cap.get(cv::CAP_PROP_FPS) << '\n';

    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cout << "没有读到帧" << '\n';
        return 1;
    }

    cv::Mat mirrored;
    cv::flip(frame, mirrored, 1);
    const bool saved = cv::imwrite("out_eg_camera.png", mirrored);
    std::cout << "saved out_eg_camera.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
