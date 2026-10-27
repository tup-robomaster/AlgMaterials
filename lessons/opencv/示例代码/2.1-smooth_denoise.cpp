#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

cv::Mat add_gaussian_noise(const cv::Mat& source, double sigma) {
    cv::Mat noisy = source.clone();
    cv::RNG rng(20261021);
    for (int y = 0; y < noisy.rows; ++y) {
        for (int x = 0; x < noisy.cols; ++x) {
            cv::Vec3b& pixel = noisy.at<cv::Vec3b>(y, x);
            for (int c = 0; c < 3; ++c) {
                pixel[c] = cv::saturate_cast<uchar>(
                    static_cast<double>(pixel[c]) + rng.gaussian(sigma));
            }
        }
    }
    return noisy;
}

cv::Mat add_salt_pepper(const cv::Mat& source, int count) {
    cv::Mat noisy = source.clone();
    cv::RNG rng(20261022);
    for (int i = 0; i < count; ++i) {
        const int x = rng.uniform(1, noisy.cols - 1);
        const int y = rng.uniform(1, noisy.rows - 1);
        const cv::Scalar color =
            (i % 2 == 0) ? cv::Scalar(0, 0, 0) : cv::Scalar(255, 255, 255);
        cv::rectangle(noisy, cv::Rect(x - 1, y - 1, 3, 3), color,
                      cv::FILLED);
    }
    return noisy;
}

double mean_abs_diff(const cv::Mat& a, const cv::Mat& b) {
    cv::Mat diff;
    cv::absdiff(a, b, diff);
    cv::Mat gray;
    cv::cvtColor(diff, gray, cv::COLOR_BGR2GRAY);
    return cv::mean(gray)[0];
}

double measure_ms(const cv::Mat& input, int mode) {
    cv::Mat output;
    const int repeats = 5;
    const int64 start = cv::getTickCount();
    for (int i = 0; i < repeats; ++i) {
        if (mode == 0) {
            cv::blur(input, output, cv::Size(5, 5));
        } else if (mode == 1) {
            cv::GaussianBlur(input, output, cv::Size(5, 5), 0);
        } else if (mode == 2) {
            cv::medianBlur(input, output, 5);
        } else {
            cv::bilateralFilter(input, output, 9, 50, 50);
        }
    }
    const double ticks = static_cast<double>(cv::getTickCount() - start);
    return ticks * 1000.0 / cv::getTickFrequency() / repeats;
}

void put_label(cv::Mat& panel, const cv::Mat& cell, int col, int row,
               const std::string& text) {
    const int cell_width = 300;
    const int cell_height = 340;
    const int label_height = 28;
    const int x = col * cell_width;
    const int y = row * (cell_height + label_height);
    cv::Mat resized;
    cv::resize(cell, resized, cv::Size(cell_width, cell_height), 0, 0,
               cv::INTER_AREA);
    resized.copyTo(panel(cv::Rect(x, y + label_height, cell_width,
                                  cell_height)));
    cv::rectangle(panel, cv::Rect(x, y, cell_width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, text, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);
}

}  // namespace

int main() {
    const std::string path = "../素材/synth_balls.png";
    cv::Mat clean = cv::imread(path, cv::IMREAD_COLOR);
    if (clean.empty()) {
        std::cout << "无法读取：" << path << '\n';
        return 1;
    }

    const cv::Mat gaussian_noise = add_gaussian_noise(clean, 14.0);
    const cv::Mat salt_pepper =
        add_salt_pepper(clean, clean.rows * clean.cols / 300);

    cv::Mat blur;
    cv::Mat gaussian;
    cv::Mat median;
    cv::Mat bilateral;
    cv::blur(gaussian_noise, blur, cv::Size(5, 5));
    cv::GaussianBlur(gaussian_noise, gaussian, cv::Size(5, 5), 0);
    cv::medianBlur(salt_pepper, median, 5);
    cv::bilateralFilter(gaussian_noise, bilateral, 9, 50, 50);

    std::cout << "clean to gaussian noise mae="
              << cv::format("%.2f", mean_abs_diff(clean, gaussian_noise))
              << '\n';
    std::cout << "clean to salt&pepper mae="
              << cv::format("%.2f", mean_abs_diff(clean, salt_pepper))
              << '\n';
    std::cout << "blur mae="
              << cv::format("%.2f", mean_abs_diff(clean, blur)) << '\n';
    std::cout << "gaussian mae="
              << cv::format("%.2f", mean_abs_diff(clean, gaussian)) << '\n';
    std::cout << "median on salt&pepper mae="
              << cv::format("%.2f", mean_abs_diff(clean, median)) << '\n';
    std::cout << "bilateral mae="
              << cv::format("%.2f", mean_abs_diff(clean, bilateral)) << '\n';

    std::cout << "time blur="
              << cv::format("%.2f", measure_ms(gaussian_noise, 0)) << " ms\n";
    std::cout << "time gaussian="
              << cv::format("%.2f", measure_ms(gaussian_noise, 1)) << " ms\n";
    std::cout << "time median="
              << cv::format("%.2f", measure_ms(salt_pepper, 2)) << " ms\n";
    std::cout << "time bilateral="
              << cv::format("%.2f", measure_ms(gaussian_noise, 3))
              << " ms\n";

    cv::Mat panel(2 * (340 + 28), 4 * 300, CV_8UC3, cv::Scalar(255, 255, 255));
    put_label(panel, clean, 0, 0, "clean");
    put_label(panel, gaussian_noise, 1, 0, "gaussian noise");
    put_label(panel, salt_pepper, 2, 0, "salt & pepper");
    put_label(panel, blur, 3, 0, "blur 5x5");
    put_label(panel, gaussian, 0, 1, "gaussian 5x5");
    put_label(panel, median, 1, 1, "median 5 on salt");
    put_label(panel, bilateral, 2, 1, "bilateral 9");
    put_label(panel, clean, 3, 1, "clean (reference)");

    const bool saved = cv::imwrite("out_smooth.png", panel);
    std::cout << "saved out_smooth.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
