#include <iostream>
#include <string>
#include <vector>

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

int contour_count(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    return static_cast<int>(contours.size());
}

}  // namespace

int main() {
    const std::string path = "../素材/synth_balls.png";
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：" << path << '\n';
        return 1;
    }

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat clean_mask;
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(85, 255, 255),
                clean_mask);

    cv::Mat noisy = clean_mask.clone();
    cv::RNG rng(20261023);
    for (int i = 0; i < 400; ++i) {
        const int x = rng.uniform(0, noisy.cols - 3);
        const int y = rng.uniform(0, noisy.rows - 3);
        cv::rectangle(noisy, cv::Rect(x, y, 3, 3), cv::Scalar(255),
                      cv::FILLED);
    }
    for (int i = 0; i < 400; ++i) {
        const int x = rng.uniform(0, noisy.cols - 5);
        const int y = rng.uniform(0, noisy.rows - 5);
        cv::rectangle(noisy, cv::Rect(x, y, 5, 5), cv::Scalar(0),
                      cv::FILLED);
    }

    const cv::Mat kernel =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
    cv::Mat eroded;
    cv::Mat dilated;
    cv::Mat opened;
    cv::Mat closed;
    cv::Mat gradient;
    cv::Mat tophat;
    cv::Mat blackhat;
    cv::erode(noisy, eroded, kernel);
    cv::dilate(noisy, dilated, kernel);
    cv::morphologyEx(noisy, opened, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(noisy, closed, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(noisy, gradient, cv::MORPH_GRADIENT, kernel);
    cv::morphologyEx(noisy, tophat, cv::MORPH_TOPHAT, kernel);
    cv::morphologyEx(noisy, blackhat, cv::MORPH_BLACKHAT, kernel);

    struct Row {
        const char* name;
        const cv::Mat* mask;
        bool binary;
    };
    const Row rows[] = {
        {"noisy", &noisy, true},     {"erode", &eroded, true},
        {"dilate", &dilated, true},  {"open", &opened, true},
        {"close", &closed, true},    {"gradient", &gradient, false},
        {"tophat", &tophat, false},  {"blackhat", &blackhat, false},
        {"clean", &clean_mask, true}};
    for (const Row& row : rows) {
        std::cout << row.name << " nonzero="
                  << cv::countNonZero(*row.mask);
        if (row.binary) {
            std::cout << " contours=" << contour_count(*row.mask);
        }
        std::cout << '\n';
    }

    cv::Mat panel(3 * (340 + 28), 3 * 300, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    add_cell(panel, noisy, 0, "noisy mask", 3, 300, 340);
    add_cell(panel, eroded, 1, "erode 7x7", 3, 300, 340);
    add_cell(panel, dilated, 2, "dilate 7x7", 3, 300, 340);
    add_cell(panel, opened, 3, "open 7x7", 3, 300, 340);
    add_cell(panel, closed, 4, "close 7x7", 3, 300, 340);
    add_cell(panel, gradient, 5, "gradient", 3, 300, 340);
    add_cell(panel, tophat, 6, "tophat", 3, 300, 340);
    add_cell(panel, blackhat, 7, "blackhat", 3, 300, 340);
    add_cell(panel, clean_mask, 8, "clean mask", 3, 300, 340);

    const bool saved = cv::imwrite("out_morph.png", panel);
    std::cout << "saved out_morph.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
