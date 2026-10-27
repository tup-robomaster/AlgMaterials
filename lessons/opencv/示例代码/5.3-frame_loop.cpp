#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char** argv) {
    bool show_window = true;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--no-window") == 0) {
            show_window = false;
        }
    }

    cv::VideoCapture cap("../素材/cube.mp4", cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cout << "无法打开视频" << '\n';
        return 1;
    }

    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int total = cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));
    std::cout << "opened=1 total=" << total << " fps=" << std::fixed
              << std::setprecision(2) << fps << '\n';

    const int delay = std::max(1, cvRound(1000.0 / fps));
    const int64 start = cv::getTickCount();
    int processed = 0;
    bool paused = false;

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        ++processed;

        const int position = cvRound(cap.get(cv::CAP_PROP_POS_FRAMES));
        const int timestamp = cvRound(cap.get(cv::CAP_PROP_POS_MSEC));
        if (position == 1 || position % 30 == 0) {
            std::cout << "sample frame=" << position << " pos=" << position
                      << " timestamp_ms=" << timestamp << '\n';
        }

        if (show_window) {
            cv::imshow("frame loop", frame);
            const int key = cv::waitKey(paused ? 0 : delay);
            if (key == 27 || key == 'q') {
                break;
            }
            if (key == ' ') {
                paused = !paused;
            }
        }
    }

    const int64 end = cv::getTickCount();
    const double elapsed_ms =
        1000.0 * static_cast<double>(end - start) / cv::getTickFrequency();
    const double average_fps =
        elapsed_ms > 0.0 ? processed * 1000.0 / elapsed_ms : 0.0;

    std::cout << "processed=" << processed << " elapsed_ms=" << std::fixed
              << std::setprecision(1) << elapsed_ms
              << " avg_fps=" << std::setprecision(2) << average_fps << '\n';
    if (show_window) {
        cv::destroyAllWindows();
    }
    return 0;
}
