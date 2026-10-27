#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

void label(cv::Mat& frame, const std::string& text) {
    cv::rectangle(frame, cv::Rect(0, 0, frame.cols, 34),
                  cv::Scalar(20, 20, 20), cv::FILLED);
    cv::putText(frame, text, cv::Point(10, 24), cv::FONT_HERSHEY_SIMPLEX,
                0.65, cv::Scalar(245, 245, 245), 1, cv::LINE_AA);
}

}  // namespace

int main() {
    const std::string path = "../素材/synth_armor.mp4";
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) {
        std::cout << "无法打开 " << path << '\n';
        return 1;
    }

    const int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int frames = cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));

    cap.set(cv::CAP_PROP_POS_FRAMES, 0);
    cv::Mat first;
    cap >> first;
    const bool first_saved =
        !first.empty() && cv::imwrite("../素材/out_armor_first.png", first);

    const std::vector<int> samples = {0, 40, 150, 240};
    std::vector<cv::Mat> tiles;
    for (int index : samples) {
        cap.set(cv::CAP_PROP_POS_FRAMES, index);
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            continue;
        }
        label(frame, "frame " + std::to_string(index));
        tiles.push_back(frame);
    }

    cv::Mat storyboard;
    if (tiles.size() == 4) {
        cv::Mat top;
        cv::Mat bottom;
        cv::hconcat(std::vector<cv::Mat>{tiles[0], tiles[1]}, top);
        cv::hconcat(std::vector<cv::Mat>{tiles[2], tiles[3]}, bottom);
        cv::vconcat(top, bottom, storyboard);
    }
    const bool story_saved = !storyboard.empty() &&
                             cv::imwrite("../素材/out_armor_story.png",
                                        storyboard);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "width = " << width << " height = " << height << '\n';
    std::cout << "fps = " << fps << " frames = " << frames << '\n';
    std::cout << "duration_s = " << frames / fps << '\n';
    std::cout << "first_saved = " << (first_saved ? "yes" : "no") << '\n';
    std::cout << "story_frames = " << tiles.size() << '\n';
    std::cout << "story_saved = " << (story_saved ? "yes" : "no");
    if (!storyboard.empty()) {
        std::cout << " " << storyboard.cols << "x" << storyboard.rows;
    }
    std::cout << '\n';
    return 0;
}
