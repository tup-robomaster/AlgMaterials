#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

void add_cell(cv::Mat& panel, const cv::Mat& image, int index,
              const std::string& label, int columns, int cell_width,
              int cell_height) {
    const int label_height = 28;
    const int col = index % columns;
    const int row = index / columns;
    const int x = col * cell_width;
    const int y = row * (cell_height + label_height);
    cv::Mat color;
    if (image.channels() == 1) {
        cv::cvtColor(image, color, cv::COLOR_GRAY2BGR);
    } else {
        color = image;
    }
    cv::Mat resized;
    cv::resize(color, resized, cv::Size(cell_width, cell_height), 0, 0,
               cv::INTER_LINEAR);
    resized.copyTo(panel(cv::Rect(x, y + label_height, cell_width,
                                  cell_height)));
    cv::rectangle(panel, cv::Rect(x, y, cell_width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);
}

int count_above(const cv::Mat& image, int value) {
    cv::Mat binary;
    cv::threshold(image, binary, value, 255, cv::THRESH_BINARY);
    return cv::countNonZero(binary);
}

}  // namespace

int main() {
    const std::string path = "../素材/synth_balls.png";
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：" << path << '\n';
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 0);

    cv::Mat sobel_x16;
    cv::Mat sobel_y16;
    cv::Sobel(blurred, sobel_x16, CV_16S, 1, 0, 3);
    cv::Sobel(blurred, sobel_y16, CV_16S, 0, 1, 3);
    cv::Mat sobel_x;
    cv::Mat sobel_y;
    cv::convertScaleAbs(sobel_x16, sobel_x);
    cv::convertScaleAbs(sobel_y16, sobel_y);

    cv::Mat magnitude;
    cv::addWeighted(sobel_x, 0.5, sobel_y, 0.5, 0.0, magnitude);

    cv::Mat laplacian16;
    cv::Laplacian(blurred, laplacian16, CV_16S, 3);
    cv::Mat laplacian;
    cv::convertScaleAbs(laplacian16, laplacian);

    cv::Mat canny_low;
    cv::Mat canny_high;
    cv::Canny(blurred, canny_low, 50, 150, 3);
    cv::Canny(blurred, canny_high, 100, 200, 3);

    std::cout << "sobel x >50=" << count_above(sobel_x, 50) << '\n';
    std::cout << "sobel y >50=" << count_above(sobel_y, 50) << '\n';
    std::cout << "magnitude >50=" << count_above(magnitude, 50) << '\n';
    std::cout << "laplacian >50=" << count_above(laplacian, 50) << '\n';
    std::cout << "canny 50/150=" << cv::countNonZero(canny_low) << '\n';
    std::cout << "canny 100/200=" << cv::countNonZero(canny_high) << '\n';

    // Canny 输出是单像素宽的线，缩到三分之一后会在缩略图里断开。
    // 面板里用的副本先做 3x3 膨胀；上面的计数仍是膨胀前的。
    const cv::Mat view_kernel =
        cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat canny_low_view;
    cv::Mat canny_high_view;
    cv::dilate(canny_low, canny_low_view, view_kernel);
    cv::dilate(canny_high, canny_high_view, view_kernel);

    cv::Mat panel(3 * (340 + 28), 3 * 300, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    add_cell(panel, gray, 0, "gray", 3, 300, 340);
    add_cell(panel, sobel_x, 1, "sobel x", 3, 300, 340);
    add_cell(panel, sobel_y, 2, "sobel y", 3, 300, 340);
    add_cell(panel, magnitude, 3, "sobel magnitude", 3, 300, 340);
    add_cell(panel, laplacian, 4, "laplacian", 3, 300, 340);
    add_cell(panel, canny_low_view, 5, "canny 50/150 (3x3 view)", 3, 300,
             340);
    add_cell(panel, canny_high_view, 6, "canny 100/200 (3x3 view)", 3, 300,
             340);
    add_cell(panel, blurred, 7, "blurred input", 3, 300, 340);
    add_cell(panel, gray, 8, "gray (reference)", 3, 300, 340);

    const bool saved = cv::imwrite("out_edges.png", panel);
    std::cout << "saved out_edges.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
