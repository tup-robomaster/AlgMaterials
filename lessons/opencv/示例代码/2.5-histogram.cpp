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

cv::Mat compute_hist(const cv::Mat& gray) {
    cv::Mat hist;
    const int channels[] = {0};
    const int hist_size[] = {256};
    const float range[] = {0.0F, 256.0F};
    const float* ranges[] = {range};
    cv::calcHist(&gray, 1, channels, cv::Mat(), hist, 1, hist_size, ranges);
    return hist;
}

cv::Mat draw_hist(const cv::Mat& hist, int width, int height) {
    const int margin = 30;
    const int plot_width = width - margin - 10;
    const int plot_height = height - margin - 10;
    cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::line(canvas, cv::Point(margin, height - margin),
             cv::Point(width - 10, height - margin), cv::Scalar(90, 90, 90),
             1);
    cv::line(canvas, cv::Point(margin, 10),
             cv::Point(margin, height - margin), cv::Scalar(90, 90, 90), 1);

    // 背景像素往往集中在一两个灰度级，直接按最大值归一化会只剩一根尖峰。
    // 取对数后再归一化到 0..1，纵轴标注为 log(count)。
    cv::Mat hist_log;
    cv::log(hist + 1.0, hist_log);
    cv::Mat hist_norm;
    cv::normalize(hist_log, hist_norm, 0.0, 1.0, cv::NORM_MINMAX);

    std::vector<cv::Point> curve;
    for (int i = 0; i < 256; ++i) {
        const int x = margin + i * plot_width / 255;
        const int y = height - margin -
                      cvRound(hist_norm.at<float>(i) * plot_height);
        curve.emplace_back(x, y);
    }
    std::vector<cv::Point> area = curve;
    area.emplace_back(width - 10, height - margin);
    area.emplace_back(margin, height - margin);
    cv::fillPoly(canvas, std::vector<std::vector<cv::Point>>{area},
                 cv::Scalar(225, 225, 225), cv::LINE_AA);
    cv::polylines(canvas, curve, false, cv::Scalar(60, 60, 60), 2,
                  cv::LINE_AA);

    cv::putText(canvas, "0", cv::Point(margin - 4, height - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(60, 60, 60), 1,
                cv::LINE_AA);
    cv::putText(canvas, "255", cv::Point(width - 34, height - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(60, 60, 60), 1,
                cv::LINE_AA);
    cv::putText(canvas, "log(count)", cv::Point(margin + 6, 22),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(120, 120, 120), 1,
                cv::LINE_AA);
    return canvas;
}

void print_stats(const std::string& name, const cv::Mat& gray,
                 const cv::Mat& hist) {
    cv::Scalar mean;
    cv::Scalar stddev;
    cv::meanStdDev(gray, mean, stddev);
    double peak = 0.0;
    cv::Point peak_loc;
    cv::minMaxLoc(hist, nullptr, &peak, nullptr, &peak_loc);
    std::cout << name << " mean=" << cv::format("%.2f", mean[0])
              << " stddev=" << cv::format("%.2f", stddev[0])
              << " peak bin=" << peak_loc.y
              << " count=" << static_cast<int>(peak) << '\n';
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

    // 把灰度压到 0.30 倍再加 70，模拟曝光不足、对比度低的输入。
    cv::Mat low;
    cv::convertScaleAbs(gray, low, 0.30, 70.0);

    cv::Mat equalized;
    cv::equalizeHist(low, equalized);

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(6.0, cv::Size(4, 4));
    cv::Mat local;
    clahe->apply(low, local);

    const cv::Mat hist_gray = compute_hist(gray);
    const cv::Mat hist_low = compute_hist(low);
    const cv::Mat hist_equalized = compute_hist(equalized);
    const cv::Mat hist_local = compute_hist(local);

    print_stats("gray", gray, hist_gray);
    print_stats("low", low, hist_low);
    print_stats("equalized", equalized, hist_equalized);
    print_stats("clahe", local, hist_local);

    double low_min = 0.0;
    double low_max = 0.0;
    cv::minMaxLoc(low, &low_min, &low_max);
    std::cout << "low range=" << cv::format("%.0f", low_min) << ".."
              << cv::format("%.0f", low_max) << '\n';

    const int cell_width = 330;
    const int cell_height = 367;
    cv::Mat panel(4 * (cell_height + 28), 2 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    add_cell(panel, gray, 0, "gray", 2, cell_width, cell_height);
    add_cell(panel, draw_hist(hist_gray, cell_width, cell_height), 1,
             "gray histogram", 2, cell_width, cell_height);
    add_cell(panel, low, 2, "low contrast (0.30x + 70)", 2, cell_width,
             cell_height);
    add_cell(panel, draw_hist(hist_low, cell_width, cell_height), 3,
             "low histogram", 2, cell_width, cell_height);
    add_cell(panel, equalized, 4, "equalizeHist", 2, cell_width,
             cell_height);
    add_cell(panel, draw_hist(hist_equalized, cell_width, cell_height), 5,
             "equalized histogram", 2, cell_width, cell_height);
    add_cell(panel, local, 6, "CLAHE clip=6.0 4x4", 2, cell_width,
             cell_height);
    add_cell(panel, draw_hist(hist_local, cell_width, cell_height), 7,
             "clahe histogram", 2, cell_width, cell_height);

    const bool saved = cv::imwrite("out_hist.png", panel);
    std::cout << "saved out_hist.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
