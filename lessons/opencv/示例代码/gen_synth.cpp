// 生成第 0、1 部分使用的合成素材 ../素材/synth_balls.png。
//
// 这张图不依赖任何外部照片：浅色渐变背景上放 22 个绿色圆球，
// 位置、颜色与尺寸都写死，重复运行得到同一张图。检查脚本以它为准。
// 运行方法（在 示例代码 目录下）：
//     g++ -std=c++17 -Wall -Wextra gen_synth.cpp -o gen_synth $(pkg-config --cflags --libs opencv4)
//     ./gen_synth

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    const int width = 1187;
    const int height = 1343;
    cv::Mat image(height, width, CV_8UC3);

    // 背景随列号缓慢变亮。第 100 行第 100 列的值是 [164,170,177]，
    // 第 100 行第 200 列的值是 [169,175,182]，第 1 部分 ROI 例子按这两点讲。
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int d = (x - 100) / 20;
            image.at<cv::Vec3b>(y, x) = cv::Vec3b(164 + d, 170 + d, 177 + d);
        }
    }

    // 22 个球：中心、半径、BGR 颜色。第一个球的中心是 (151,562)，
    // 盖住第 562 行第 151 列，第 1 部分颜色例子读取的正是这个像素。
    struct Ball {
        int x;
        int y;
        int radius;
        int b;
        int g;
        int r;
    };
    const Ball balls[] = {
        {151, 562, 50, 132, 188, 167}, {850, 170, 48, 90, 180, 110},
        {300, 300, 52, 110, 200, 140}, {600, 320, 46, 70, 160, 100},
        {900, 360, 50, 100, 190, 130}, {150, 420, 54, 120, 210, 150},
        {430, 450, 47, 80, 170, 105},  {760, 470, 51, 115, 195, 145},
        {1050, 500, 49, 95, 185, 125}, {330, 620, 53, 105, 200, 135},
        {560, 600, 46, 85, 175, 115},  {880, 620, 52, 125, 190, 150},
        {120, 760, 50, 75, 165, 105},  {400, 740, 48, 110, 180, 135},
        {700, 760, 53, 100, 205, 145}, {1000, 780, 47, 90, 190, 120},
        {280, 910, 51, 120, 200, 160}, {580, 890, 49, 80, 185, 110},
        {860, 930, 52, 105, 175, 125}, {150, 1080, 48, 95, 195, 140},
        {470, 1080, 50, 115, 210, 155}, {800, 1120, 47, 85, 180, 120},
    };

    for (const Ball& ball : balls) {
        cv::circle(image, cv::Point(ball.x, ball.y), ball.radius,
                   cv::Scalar(ball.b, ball.g, ball.r), cv::FILLED,
                   cv::LINE_AA);
    }

    const bool saved = cv::imwrite("../素材/synth_balls.png", image);
    return saved ? 0 : 1;
}
