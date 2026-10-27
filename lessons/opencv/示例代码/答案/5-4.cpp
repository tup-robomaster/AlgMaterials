#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

int main() {
    cv::VideoCapture cap("../素材/cube.mp4", cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cout << "无法打开输入视频" << '\n';
        return 1;
    }

    const int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter writer("out_ex_5_4.avi",
                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 15.0,
                           cv::Size(width, height));
    if (!writer.isOpened()) {
        std::cout << "无法创建输出视频" << '\n';
        return 1;
    }

    int written = 0;
    while (written < 20) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        cv::line(frame, cv::Point(0, 0), cv::Point(width - 1, height - 1),
                 cv::Scalar(0, 220, 255), 2, cv::LINE_AA);
        writer.write(frame);
        ++written;
    }
    writer.release();

    cv::VideoCapture reader("out_ex_5_4.avi", cv::CAP_ANY);
    if (!reader.isOpened()) {
        std::cout << "回读失败" << '\n';
        return 1;
    }
    std::cout << "written=" << written << " readback="
              << cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT)) << " size="
              << cvRound(reader.get(cv::CAP_PROP_FRAME_WIDTH)) << "x"
              << cvRound(reader.get(cv::CAP_PROP_FRAME_HEIGHT)) << " fps="
              << std::fixed << std::setprecision(2)
              << reader.get(cv::CAP_PROP_FPS) << '\n';
    return 0;
}
