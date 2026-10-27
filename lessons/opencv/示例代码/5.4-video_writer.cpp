#include <iomanip>
#include <iostream>
#include <string>

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
    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int total = cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));
    std::cout << "opened=1 input=" << width << "x" << height << " fps="
              << std::fixed << std::setprecision(2) << fps
              << " frames=" << total << '\n';

    const std::string path = "../素材/out_cube.mp4";
    cv::VideoWriter writer(path,
                           cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps,
                           cv::Size(width, height));
    std::cout << "writer opened=" << (writer.isOpened() ? 1 : 0) << '\n';
    if (!writer.isOpened()) {
        std::cout << "无法创建输出视频" << '\n';
        return 1;
    }

    int written = 0;
    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        ++written;

        cv::Mat shown = frame.clone();
        const std::string label = "frame " + std::to_string(written);
        cv::rectangle(shown, cv::Rect(0, 0, 180, 40), cv::Scalar(20, 20, 20),
                      cv::FILLED);
        cv::putText(shown, label, cv::Point(12, 28),
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(240, 240, 240),
                    2, cv::LINE_AA);
        writer.write(shown);
    }
    writer.release();
    std::cout << "written=" << written << '\n';

    cv::VideoCapture reader(path, cv::CAP_ANY);
    if (!reader.isOpened()) {
        std::cout << "回读失败" << '\n';
        return 1;
    }
    std::cout << "readback opened=1 size="
              << cvRound(reader.get(cv::CAP_PROP_FRAME_WIDTH)) << "x"
              << cvRound(reader.get(cv::CAP_PROP_FRAME_HEIGHT))
              << " fps=" << std::fixed << std::setprecision(2)
              << reader.get(cv::CAP_PROP_FPS)
              << " frames=" << cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT))
              << '\n';
    std::cout << "saved out_cube.mp4: yes" << '\n';
    return 0;
}
