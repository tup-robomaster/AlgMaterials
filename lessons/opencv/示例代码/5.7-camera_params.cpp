#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

void place(cv::Mat& panel, const cv::Mat& image, int x, int y, int width,
           int height, const std::string& label) {
    const int label_height = 28;
    cv::rectangle(panel, cv::Rect(x, y, width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);

    const int inner_width = width - 16;
    const int inner_height = height - label_height - 16;
    const double scale = std::min(static_cast<double>(inner_width) / image.cols,
                                  static_cast<double>(inner_height) / image.rows);
    const int shown_width = std::max(1, cvRound(image.cols * scale));
    const int shown_height = std::max(1, cvRound(image.rows * scale));
    cv::Mat shown;
    cv::resize(image, shown, cv::Size(shown_width, shown_height), 0, 0,
               cv::INTER_AREA);
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

cv::Mat makeScene() {
    cv::Mat scene(240, 320, CV_8UC3);
    for (int y = 0; y < scene.rows; ++y) {
        const int value = 35 + y * 90 / scene.rows;
        scene.row(y).setTo(cv::Scalar(value, value, value));
    }
    cv::rectangle(scene, cv::Rect(24, 30, 90, 70), cv::Scalar(40, 40, 210),
                  cv::FILLED);
    cv::rectangle(scene, cv::Rect(150, 40, 80, 80), cv::Scalar(40, 200, 60),
                  cv::FILLED);
    cv::circle(scene, cv::Point(95, 170), 44, cv::Scalar(230, 230, 230),
               cv::FILLED, cv::LINE_AA);
    cv::circle(scene, cv::Point(235, 175), 38, cv::Scalar(25, 25, 25),
               cv::FILLED, cv::LINE_AA);
    return scene;
}

void report(const std::string& name, const cv::Mat& image) {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat clipped;
    cv::threshold(gray, clipped, 249, 255, cv::THRESH_BINARY);
    std::cout << name << " mean=" << cvRound(cv::mean(gray)[0])
              << " clipped=" << cv::countNonZero(clipped) << '\n';
}

}  // namespace

int main() {
    const cv::Mat scene = makeScene();

    cv::Mat bright;
    scene.convertTo(bright, CV_8UC3, 4.0, 0.0);
    cv::Mat dark;
    scene.convertTo(dark, CV_8UC3, 0.25, 0.0);

    cv::RNG rng(12345);
    cv::Mat noise(scene.size(), CV_32FC3);
    rng.fill(noise, cv::RNG::NORMAL, 0.0, 18.0);
    cv::Mat scene_float;
    scene.convertTo(scene_float, CV_32FC3);
    const cv::Mat noisy_float = scene_float + noise;
    cv::Mat noisy;
    noisy_float.convertTo(noisy, CV_8UC3);

    report("normal", scene);
    report("exposure x4", bright);
    report("exposure x0.25", dark);
    report("gain high", noisy);

    cv::Mat panel(300, 1040, CV_8UC3, cv::Scalar(255, 255, 255));
    place(panel, scene, 0, 0, 260, 300, "normal");
    place(panel, bright, 260, 0, 260, 300, "exposure x4");
    place(panel, dark, 520, 0, 260, 300, "exposure x0.25");
    place(panel, noisy, 780, 0, 260, 300, "gain high + noise");
    const bool saved = cv::imwrite("../素材/out_exposure.png", panel);
    std::cout << "saved out_exposure.png: " << (saved ? "yes" : "no")
              << '\n';

    const cv::Mat mono8(2, 2, CV_8UC1, cv::Scalar(128));
    const cv::Mat mono12(2, 2, CV_16UC1, cv::Scalar(2048));
    const cv::Mat bgr8(2, 2, CV_8UC3, cv::Scalar(10, 20, 30));
    std::cout << "mono8 type=" << mono8.type()
              << " channels=" << mono8.channels() << " elem="
              << mono8.elemSize() << '\n';
    std::cout << "mono12 type=" << mono12.type()
              << " channels=" << mono12.channels() << " elem="
              << mono12.elemSize() << '\n';
    std::cout << "bgr8 type=" << bgr8.type()
              << " channels=" << bgr8.channels() << " elem="
              << bgr8.elemSize() << '\n';

    const std::vector<int> twelve = {0, 1023, 2047, 4095};
    std::cout << "mono12 to mono8:";
    for (const int value : twelve) {
        std::cout << " " << value << "->"
                  << cvRound(value * 255.0 / 4095.0);
    }
    std::cout << '\n';
    return 0;
}
