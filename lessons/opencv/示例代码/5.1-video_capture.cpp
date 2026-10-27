#include <iomanip>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

int main() {
    const std::string path = "../素材/cube.mp4";
    cv::VideoCapture cap(path, cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cout << "无法打开 " << path << '\n';
        return 1;
    }

    const double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    const double fps = cap.get(cv::CAP_PROP_FPS);
    const double frames = cap.get(cv::CAP_PROP_FRAME_COUNT);

    std::cout << "opened=1" << '\n';
    std::cout << "width=" << cvRound(width) << " height=" << cvRound(height)
              << " fps=" << std::fixed << std::setprecision(2) << fps
              << " frames=" << cvRound(frames) << '\n';

    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cout << "读取第一帧失败" << '\n';
        return 1;
    }
    std::cout << "first frame=" << frame.cols << "x" << frame.rows << '\n';
    std::cout << "position=" << cvRound(cap.get(cv::CAP_PROP_POS_FRAMES))
              << '\n';

    cv::Mat shown = frame.clone();
    cv::Mat bar(74, 420, CV_8UC3, cv::Scalar(20, 20, 20));
    bar.copyTo(shown(cv::Rect(14, 14, bar.cols, bar.rows)));
    cv::putText(shown,
                cv::format("%dx%d  %.2f fps  %d frames", frame.cols,
                           frame.rows, fps, cvRound(frames)),
                cv::Point(26, 46), cv::FONT_HERSHEY_SIMPLEX, 0.60,
                cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
    cv::putText(shown, "first frame", cv::Point(26, 76),
                cv::FONT_HERSHEY_SIMPLEX, 0.60, cv::Scalar(120, 220, 255), 1,
                cv::LINE_AA);

    const bool saved = cv::imwrite("../素材/out_first_frame.png", shown);
    std::cout << "saved out_first_frame.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
