#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main() {
    const std::string path = "../素材/synth_balls.png";
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cout << "无法读取：" << path << std::endl;
        return 1;
    }

    std::cout << "rows=" << image.rows << " cols=" << image.cols
              << " channels=" << image.channels()
              << " type=" << image.type() << std::endl;

    const bool saved = cv::imwrite("out_synth_balls.png", image);
    std::cout << "saved out_synth_balls.png: " << (saved ? "yes" : "no")
              << std::endl;

    cv::imshow("synth_balls", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
