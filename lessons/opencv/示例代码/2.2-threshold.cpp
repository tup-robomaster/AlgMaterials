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
               cv::INTER_AREA);
    resized.copyTo(panel(cv::Rect(x, y + label_height, cell_width,
                                  cell_height)));
    cv::rectangle(panel, cv::Rect(x, y, cell_width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);
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

    const char* names[] = {"BINARY", "BINARY_INV", "TRUNC", "TOZERO",
                           "TOZERO_INV"};
    const int types[] = {cv::THRESH_BINARY, cv::THRESH_BINARY_INV,
                         cv::THRESH_TRUNC, cv::THRESH_TOZERO,
                         cv::THRESH_TOZERO_INV};
    cv::Mat masks[5];
    for (int i = 0; i < 5; ++i) {
        cv::threshold(gray, masks[i], 190, 255, types[i]);
        std::cout << names[i] << " nonzero=" << cv::countNonZero(masks[i])
                  << '\n';
    }

    cv::Mat otsu_mask;
    const double otsu = cv::threshold(gray, otsu_mask, 0, 255,
                                      cv::THRESH_BINARY | cv::THRESH_OTSU);
    std::cout << "otsu threshold=" << cv::format("%.1f", otsu)
              << " nonzero=" << cv::countNonZero(otsu_mask) << '\n';

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat green_mask;
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(85, 255, 255),
                green_mask);
    std::cout << "green inRange nonzero=" << cv::countNonZero(green_mask)
              << '\n';

    cv::Mat uneven(gray.size(), CV_8UC1);
    for (int y = 0; y < gray.rows; ++y) {
        for (int x = 0; x < gray.cols; ++x) {
            const double scale = 0.35 + 0.65 * x / (gray.cols - 1);
            uneven.at<uchar>(y, x) = cv::saturate_cast<uchar>(
                gray.at<uchar>(y, x) * scale);
        }
    }

    cv::Mat uneven_fixed;
    cv::threshold(uneven, uneven_fixed, 128, 255, cv::THRESH_BINARY);
    std::cout << "uneven fixed nonzero=" << cv::countNonZero(uneven_fixed)
              << '\n';

    cv::Mat adaptive_mask;
    cv::adaptiveThreshold(uneven, adaptive_mask, 255,
                          cv::ADAPTIVE_THRESH_MEAN_C,
                          cv::THRESH_BINARY_INV, 151, 10);
    std::cout << "adaptive block=151 C=10 nonzero="
              << cv::countNonZero(adaptive_mask) << '\n';

    cv::Mat panel(3 * (340 + 28), 4 * 300, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    add_cell(panel, gray, 0, "gray", 4, 300, 340);
    add_cell(panel, masks[0], 1, "BINARY 190", 4, 300, 340);
    add_cell(panel, masks[1], 2, "BINARY_INV 190", 4, 300, 340);
    add_cell(panel, masks[2], 3, "TRUNC 190", 4, 300, 340);
    add_cell(panel, masks[3], 4, "TOZERO 190", 4, 300, 340);
    add_cell(panel, masks[4], 5, "TOZERO_INV 190", 4, 300, 340);
    add_cell(panel, otsu_mask, 6, "Otsu", 4, 300, 340);
    add_cell(panel, green_mask, 7, "HSV green", 4, 300, 340);
    add_cell(panel, uneven, 8, "uneven input", 4, 300, 340);
    add_cell(panel, uneven_fixed, 9, "uneven fixed 128", 4, 300, 340);
    add_cell(panel, adaptive_mask, 10, "uneven adaptive", 4, 300, 340);
    add_cell(panel, gray, 11, "gray (reference)", 4, 300, 340);

    const bool saved = cv::imwrite("out_threshold.png", panel);
    std::cout << "saved out_threshold.png: " << (saved ? "yes" : "no")
              << '\n';
    return 0;
}
