#include <iomanip>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

void report(const std::string& title, cv::VideoCapture& cap) {
    std::cout << title << " opened=" << (cap.isOpened() ? 1 : 0);
    std::cout << " width=" << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    std::cout << " height=" << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    std::cout << " frames=" << cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));
    std::cout << " fps=" << std::fixed << std::setprecision(2)
              << cap.get(cv::CAP_PROP_FPS) << '\n';
}

int main() {
    cv::VideoCapture closed;
    report("closed", closed);

    cv::VideoCapture good("../素材/cube.mp4");
    report("good", good);
    return 0;
}
