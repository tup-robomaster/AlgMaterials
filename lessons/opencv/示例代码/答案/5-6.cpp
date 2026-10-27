#include <iostream>
#include <vector>

#include <opencv2/core.hpp>

int main() {
    std::vector<uchar> buffer(10, 0);
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = static_cast<uchar>(i);
    }

    cv::Mat view(2, 3, CV_8UC1, buffer.data(), 5);
    std::cout << "rows=" << view.rows << " cols=" << view.cols
              << " step=" << view.step[0] << " total=" << view.total()
              << '\n';
    std::cout << "row0=" << static_cast<int>(view.at<uchar>(0, 0)) << " "
              << static_cast<int>(view.at<uchar>(0, 1)) << " "
              << static_cast<int>(view.at<uchar>(0, 2)) << " row1="
              << static_cast<int>(view.at<uchar>(1, 0)) << " "
              << static_cast<int>(view.at<uchar>(1, 1)) << " "
              << static_cast<int>(view.at<uchar>(1, 2)) << '\n';

    view.at<uchar>(0, 0) = 99;
    std::cout << "backing first=" << static_cast<int>(buffer[0])
              << " padding=" << static_cast<int>(buffer[3]) << '\n';
    return 0;
}
