// 生成第 4 部分用的两张合成素材：
//   synth_bars.png   红蓝灯条、横条、方块与白块，用于多颜色统计；
//   armor_frame.png  两块装甲板候选、孤立灯条与干扰块，用于配对。
// 位置、颜色与尺寸都写死，重复运行得到同一张图。

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

void drawBar(cv::Mat& image, cv::Point2f center, cv::Size2f size,
             double angle, const cv::Scalar& color) {
    cv::RotatedRect rect(center, size, angle);
    cv::Point2f points[4];
    rect.points(points);
    std::vector<cv::Point> polygon;
    for (const cv::Point2f& p : points) {
        polygon.emplace_back(cvRound(p.x), cvRound(p.y));
    }
    cv::fillConvexPoly(image, polygon, color, cv::LINE_AA);
}

void makeBars(const std::string& path) {
    cv::Mat image(700, 1000, CV_8UC3, cv::Scalar(38, 38, 42));

    const cv::Scalar red(0, 0, 230);
    const cv::Scalar blue(230, 40, 0);
    const cv::Scalar white(235, 235, 235);

    drawBar(image, cv::Point2f(180, 180), cv::Size2f(18, 170), 6, red);
    drawBar(image, cv::Point2f(280, 180), cv::Size2f(18, 170), -6, red);
    drawBar(image, cv::Point2f(480, 180), cv::Size2f(18, 150), 0, red);
    drawBar(image, cv::Point2f(720, 520), cv::Size2f(170, 18), 0, red);
    cv::rectangle(image, cv::Rect(857, 97, 46, 46), red, cv::FILLED);

    drawBar(image, cv::Point2f(650, 180), cv::Size2f(18, 170), 6, blue);
    drawBar(image, cv::Point2f(750, 180), cv::Size2f(18, 170), -6, blue);
    drawBar(image, cv::Point2f(160, 520), cv::Size2f(18, 150), 0, blue);
    drawBar(image, cv::Point2f(830, 420), cv::Size2f(170, 18), 0, blue);

    cv::rectangle(image, cv::Rect(880, 580, 40, 40), white, cv::FILLED);

    const bool saved = cv::imwrite(path, image);
    std::cout << "saved " << path << ": " << (saved ? "yes" : "no") << '\n';
}

void makeArmor(const std::string& path) {
    cv::Mat image(720, 1280, CV_8UC3, cv::Scalar(28, 28, 32));

    const cv::Scalar red(0, 0, 230);
    const cv::Scalar blue(230, 40, 0);
    const cv::Scalar panel(190, 190, 190);
    const cv::Scalar white(235, 235, 235);
    const cv::Scalar green(40, 200, 40);

    // 红色装甲板：两根灯条向中间倾斜，中间是浅色数字区。
    cv::rectangle(image, cv::Rect(350, 290, 100, 140), panel, cv::FILLED);
    drawBar(image, cv::Point2f(330, 360), cv::Size2f(18, 170), 8, red);
    drawBar(image, cv::Point2f(470, 360), cv::Size2f(18, 170), -8, red);

    // 蓝色装甲板。
    cv::rectangle(image, cv::Rect(830, 310, 100, 140), panel, cv::FILLED);
    drawBar(image, cv::Point2f(810, 380), cv::Size2f(18, 170), -8, blue);
    drawBar(image, cv::Point2f(950, 380), cv::Size2f(18, 170), 8, blue);

    // 孤立灯条与干扰块。
    drawBar(image, cv::Point2f(150, 170), cv::Size2f(18, 150), 0, red);
    drawBar(image, cv::Point2f(620, 360), cv::Size2f(18, 160), 0, red);
    drawBar(image, cv::Point2f(640, 620), cv::Size2f(170, 18), 0, red);
    cv::rectangle(image, cv::Rect(1077, 147, 46, 46), red, cv::FILLED);
    cv::rectangle(image, cv::Rect(1130, 580, 40, 40), white, cv::FILLED);
    drawBar(image, cv::Point2f(100, 620), cv::Size2f(18, 150), 0, green);

    const bool saved = cv::imwrite(path, image);
    std::cout << "saved " << path << ": " << (saved ? "yes" : "no") << '\n';
}

}  // namespace

int main() {
    makeBars("../素材/synth_bars.png");
    makeArmor("../素材/armor_frame.png");
    return 0;
}
