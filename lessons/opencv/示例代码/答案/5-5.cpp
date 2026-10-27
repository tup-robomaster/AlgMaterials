#include <cstddef>
#include <deque>
#include <iostream>

#include <opencv2/core.hpp>

int main() {
    const std::size_t capacity = 4;
    std::deque<cv::Mat> queue;
    int dropped = 0;

    for (int id = 0; id < 7; ++id) {
        cv::Mat buffer(1, 1, CV_8UC1, cv::Scalar(id));
        queue.push_back(buffer.clone());
        if (queue.size() > capacity) {
            queue.pop_front();
            ++dropped;
        }
        std::cout << "push=" << id << " size=" << queue.size()
                  << " dropped=" << dropped << '\n';
    }

    while (!queue.empty()) {
        const int id = static_cast<int>(queue.front().at<uchar>(0, 0));
        queue.pop_front();
        std::cout << "pop=" << id << " left=" << queue.size() << '\n';
    }
    return 0;
}
