#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

void put_label(cv::Mat& tile, const std::string& text) {
    cv::putText(tile, text, cv::Point(14, 36), cv::FONT_HERSHEY_SIMPLEX,
                0.9, cv::Scalar(0, 0, 0), 5, cv::LINE_AA);
    cv::putText(tile, text, cv::Point(12, 34), cv::FONT_HERSHEY_SIMPLEX,
                0.9, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
}

}  // namespace

int main() {
    const std::string path = "../素材/real_armor.mp4";
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) {
        std::cerr << "无法打开 real_armor.mp4\n";
        return 1;
    }

    const double fps = cap.get(cv::CAP_PROP_FPS);
    const double count = cap.get(cv::CAP_PROP_FRAME_COUNT);
    const int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    if (fps <= 0.0 || count <= 0.0) {
        std::cerr << "视频属性无效\n";
        return 1;
    }

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "size = " << width << 'x' << height << '\n';
    std::cout << "fps = " << fps << '\n';
    std::cout << "frames = " << static_cast<int>(count) << '\n';
    std::cout << "duration = " << count / fps << " s\n";

    const double seconds[] = {25.0, 50.0, 80.0, 105.0};
    std::vector<int> targets;
    for (double s : seconds) {
        targets.push_back(static_cast<int>(std::lround(s * fps)));
    }

    std::vector<cv::Mat> tiles;
    cv::Mat frame;
    std::size_t next = 0;
    for (int index = 0; index <= targets.back() && cap.read(frame); ++index) {
        if (next < targets.size() && index == targets[next]) {
            cv::Mat tile = frame.clone();
            put_label(tile, std::to_string(static_cast<int>(seconds[next])) +
                                 " s");
            tiles.push_back(tile);
            ++next;
        }
    }
    if (tiles.size() != 4) {
        std::cerr << "抽取的帧数不足\n";
        return 1;
    }

    cv::Mat top;
    cv::Mat bottom;
    cv::Mat panel;
    cv::hconcat(tiles[0], tiles[1], top);
    cv::hconcat(tiles[2], tiles[3], bottom);
    cv::vconcat(top, bottom, panel);

    const bool saved = cv::imwrite("../素材/out_armor_real.png", panel);
    std::cout << "saved out_armor_real.png " << panel.cols << 'x'
              << panel.rows << ": " << (saved ? "yes" : "no") << '\n';
    return saved ? 0 : 1;
}
