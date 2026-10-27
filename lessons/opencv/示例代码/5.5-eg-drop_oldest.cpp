#include <cstddef>
#include <deque>
#include <iostream>

#include <opencv2/core.hpp>

int main() {
    const std::size_t capacity = 3;
    std::deque<cv::Mat> queue;
    int dropped = 0;

    for (int id = 0; id < 5; ++id) {
        cv::Mat frame(1, 1, CV_8UC1, cv::Scalar(id));
        queue.push_back(frame.clone());
        if (queue.size() > capacity) {
            queue.pop_front();
            ++dropped;
        }
        std::cout << "pushed=" << id << " size=" << queue.size()
                  << " dropped=" << dropped << '\n';
    }

    while (!queue.empty()) {
        const cv::Mat frame = queue.front();
        queue.pop_front();
        std::cout << "pop=" << static_cast<int>(frame.at<uchar>(0, 0))
                  << " left=" << queue.size() << '\n';
    }
    return 0;
}
