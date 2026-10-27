#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    const std::string path = "../素材/synth_balls.png";
    cv::Mat gray = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if (gray.empty()) {
        std::cout << "无法读取：" << path << '\n';
        return 1;
    }

    std::cout << "gray: rows=" << gray.rows << " cols=" << gray.cols
              << " channels=" << gray.channels()
              << " type=" << gray.type() << '\n';

    const bool saved = cv::imwrite("out_ex_1_2.png", gray);
    std::cout << "saved out_ex_1_2.png: " << (saved ? "yes" : "no")
              << '\n';

    cv::Mat reloaded = cv::imread("out_ex_1_2.png", cv::IMREAD_UNCHANGED);
    if (reloaded.empty()) {
        std::cout << "重新读取失败" << '\n';
        return 1;
    }
    std::cout << "reloaded channels=" << reloaded.channels()
              << " type=" << reloaded.type() << '\n';

    return 0;
}
