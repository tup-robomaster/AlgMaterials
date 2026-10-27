#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

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
               cv::INTER_AREA);
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

}  // namespace

int main(int argc, char** argv) {
    bool try_camera = true;
    bool show_window = true;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--no-camera") == 0) {
            try_camera = false;
        } else if (std::strcmp(argv[i], "--no-window") == 0) {
            show_window = false;
        }
    }

    cv::VideoCapture cap;
    std::string mode = "video";
    if (try_camera) {
        // Linux 上优先用 V4L2。CAP_ANY 可能选中 GStreamer，部分 GStreamer
        // 版本在设置宽高时会停掉采集管线，之后所有帧都为空。
        cap.open(0, cv::CAP_V4L2);
        if (!cap.isOpened()) {
            cap.open(0, cv::CAP_ANY);
        }
        if (cap.isOpened()) {
            mode = "camera";
        } else {
            std::cout << "camera open failed, using video" << '\n';
        }
    }

    if (!cap.isOpened()) {
        mode = "video";
        cap.open("../素材/cube.mp4", cv::CAP_ANY);
    }
    if (!cap.isOpened()) {
        std::cout << "无法打开视频" << '\n';
        return 1;
    }

    const bool set_width = cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    const bool set_height = cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    const bool set_fps = cap.set(cv::CAP_PROP_FPS, 30);

    cv::Mat last;
    int frames = 0;
    for (int i = 0; i < 5; ++i) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        last = frame.clone();
        ++frames;
    }

    if (frames == 0 && mode == "camera") {
        std::cout << "camera gave no frame, using video" << '\n';
        cap.release();
        cap.open("../素材/cube.mp4", cv::CAP_ANY);
        mode = "video";
        for (int i = 0; i < 5; ++i) {
            cv::Mat frame;
            cap >> frame;
            if (frame.empty()) {
                break;
            }
            last = frame.clone();
            ++frames;
        }
    }

    const double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    const double fps = cap.get(cv::CAP_PROP_FPS);
    std::cout << "mode=" << mode << '\n';
    std::cout << "set width=" << set_width << " height=" << set_height
              << " fps=" << set_fps << '\n';
    std::cout << "requested=1280x720 fps=30.00" << '\n';
    std::cout << "actual=" << cvRound(width) << "x" << cvRound(height)
              << " fps=" << std::fixed << std::setprecision(2) << fps << '\n';
    std::cout << "frames=" << frames << '\n';

    if (last.empty()) {
        std::cout << "没有读到帧" << '\n';
        return 1;
    }

    cv::Mat mirrored;
    cv::flip(last, mirrored, 1);
    const bool saved_jpg = cv::imwrite("../素材/snapshot.jpg", mirrored);

    cv::Mat panel(520, 1280, CV_8UC3, cv::Scalar(255, 255, 255));
    place(panel, last, 0, 0, 640, 520, "last captured frame");
    place(panel, mirrored, 640, 0, 640, 520, "mirrored snapshot");
    const bool saved_png = cv::imwrite("../素材/out_camera.png", panel);

    std::cout << "saved snapshot.jpg: " << (saved_jpg ? "yes" : "no") << '\n';
    std::cout << "saved out_camera.png: " << (saved_png ? "yes" : "no")
              << '\n';

    if (show_window) {
        if (mode == "camera") {
            cv::imshow("snapshot", mirrored);
        } else {
            cv::imshow("camera fallback", panel);
        }
        std::cout << "press any key in the window to close" << '\n';
        cv::waitKey(0);
    }
    return 0;
}
