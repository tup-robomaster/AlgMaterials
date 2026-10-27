#include <iostream>

#include <opencv2/core.hpp>

int main() {
    cv::Mat image(2, 3, CV_8UC1, cv::Scalar(10));

    std::cout << "rows=" << image.rows << " cols=" << image.cols
              << " channels=" << image.channels()
              << " depth=" << image.depth()
              << " type=" << image.type() << '\n';
    std::cout << "step=" << image.step << " elemSize=" << image.elemSize()
              << " continuous=" << (image.isContinuous() ? "yes" : "no")
              << '\n';

    cv::Mat alias = image;
    std::cout << "alias shares data: "
              << (alias.data == image.data ? "yes" : "no") << '\n';

    alias.at<uchar>(0, 0) = 99;
    std::cout << "image(0,0) after alias: "
              << static_cast<int>(image.at<uchar>(0, 0)) << '\n';

    cv::Mat clone = image.clone();
    std::cout << "clone shares data: "
              << (clone.data == image.data ? "yes" : "no") << '\n';

    clone.at<uchar>(0, 0) = 42;
    std::cout << "image(0,0) after clone: "
              << static_cast<int>(image.at<uchar>(0, 0)) << '\n';
    std::cout << "clone(0,0): " << static_cast<int>(clone.at<uchar>(0, 0))
              << '\n';

    cv::Mat copied;
    image.copyTo(copied);
    std::cout << "copyTo shares data: "
              << (copied.data == image.data ? "yes" : "no") << '\n';

    return 0;
}
