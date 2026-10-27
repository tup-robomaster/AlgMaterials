#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

int main() {
    cv::VideoCapture cap("../素材/synth_armor.mp4");
    if (!cap.isOpened()) {
        std::cout << "open failed\n";
        return 1;
    }
    const int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int frames = cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "video = " << width << 'x' << height << '\n';
    std::cout << "fps = " << fps << " frames = " << frames << '\n';
    std::cout << "duration_s = " << frames / fps << '\n';
    return 0;
}
