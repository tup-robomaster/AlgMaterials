#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image(4, 4, CV_8UC1, cv::Scalar(120));
    image.at<uchar>(0, 0) = 100;

    double low_min = 0.0;
    double low_max = 0.0;
    cv::minMaxLoc(image, &low_min, &low_max);

    cv::Mat equalized;
    cv::equalizeHist(image, equalized);
    double high_min = 0.0;
    double high_max = 0.0;
    cv::minMaxLoc(equalized, &high_min, &high_max);

    std::cout << "before=" << low_min << ".." << low_max << '\n';
    std::cout << "after=" << high_min << ".." << high_max << '\n';
    std::cout << "equalized(0,0)="
              << static_cast<int>(equalized.at<uchar>(0, 0)) << '\n';
    std::cout << "equalized(1,1)="
              << static_cast<int>(equalized.at<uchar>(1, 1)) << '\n';
    return 0;
}
