#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

void writeVideo(const std::string& path, int fourcc) {
    cv::VideoWriter writer(path, fourcc, 20.0, cv::Size(96, 64));
    std::cout << path << " writer=" << (writer.isOpened() ? 1 : 0);
    if (!writer.isOpened()) {
        std::cout << '\n';
        return;
    }

    for (int i = 0; i < 6; ++i) {
        cv::Mat frame(64, 96, CV_8UC3, cv::Scalar(30, 30, 30));
        cv::rectangle(frame, cv::Rect(4 + i * 12, 20, 24, 24),
                      cv::Scalar(40, 200, 240), cv::FILLED);
        writer.write(frame);
    }
    writer.release();

    cv::VideoCapture reader(path, cv::CAP_ANY);
    std::cout << " read=" << (reader.isOpened() ? 1 : 0)
              << " size=" << cvRound(reader.get(cv::CAP_PROP_FRAME_WIDTH))
              << "x" << cvRound(reader.get(cv::CAP_PROP_FRAME_HEIGHT))
              << " frames="
              << cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT)) << '\n';
}

}  // namespace

int main() {
    writeVideo("out_eg_mjpg.avi",
               cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    writeVideo("out_eg_mp4v.mp4",
               cv::VideoWriter::fourcc('m', 'p', '4', 'v'));
    return 0;
}
