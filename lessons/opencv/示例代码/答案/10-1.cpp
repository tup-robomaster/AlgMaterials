#include <iomanip>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>

namespace {

void print(const std::string& name, const cv::Mat& image) {
    std::cout << name << " (" << image.rows << "x" << image.cols << ")\n";
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            std::cout << std::setw(4)
                      << static_cast<int>(image.at<uchar>(row, col));
        }
        std::cout << '\n';
    }
}

}  // namespace

int main() {
    cv::Mat src = (cv::Mat_<uchar>(4, 5) << 10, 20, 30, 40, 50, 60, 70, 80,
                   90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200);
    cv::Mat roi = src(cv::Rect(1, 1, 3, 2));
    cv::Mat shallow = src;
    cv::Mat deep = src.clone();

    roi.setTo(0);
    shallow.at<uchar>(0, 0) = 255;
    deep.at<uchar>(3, 4) = 1;

    print("src", src);
    print("roi", roi);
    print("deep", deep);
    std::cout << "src.data == shallow.data: "
              << (src.data == shallow.data ? "yes" : "no") << '\n';
    std::cout << "src.data == deep.data: "
              << (src.data == deep.data ? "yes" : "no") << '\n';
    std::cout << "roi is a view: "
              << (roi.data >= src.data && roi.data < src.data + src.total()
                      ? "yes"
                      : "no")
              << '\n';
    return 0;
}
