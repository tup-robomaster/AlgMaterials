// 生成第 2、3 部分使用的合成素材。
//
// 所有场景都由固定坐标与固定颜色画成，重复运行得到同一批图片：
//   synth_balls_1-4.png   不同亮度、干扰色块与白球的绿球场景
//   synth_blocks.png      不同旋转角度的正方形、长方形与一个圆
//   synth_touching.png    五对接触的绿球与两个单独的绿球
//   synth_card.png        透视变形后的卡片，用于几何矫正
//   synth_match_template.png / synth_match_scene.png
//                         多尺度模板匹配的模板与场景
//
// 运行方法（在 示例代码 目录下）：
//     g++ -std=c++17 -Wall -Wextra gen_part23.cpp -o gen_part23 $(pkg-config --cflags --libs opencv4)
//     ./gen_part23

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

cv::Mat make_background(int width, int height, const cv::Scalar& base) {
    cv::Mat image(height, width, CV_8UC3);
    for (int x = 0; x < width; ++x) {
        const int d = (x - 60) / 30;
        const cv::Scalar color(
            std::clamp(static_cast<int>(base[0]) + d, 0, 255),
            std::clamp(static_cast<int>(base[1]) + d, 0, 255),
            std::clamp(static_cast<int>(base[2]) + d, 0, 255));
        image.col(x).setTo(color);
    }
    return image;
}

void draw_ball(cv::Mat& image, int x, int y, int radius,
               const cv::Scalar& color, bool highlight = false) {
    cv::circle(image, cv::Point(x, y), radius, color, cv::FILLED, cv::LINE_AA);
    if (highlight) {
        cv::circle(image, cv::Point(x - radius / 3, y - radius / 3),
                   std::max(4, radius / 5), cv::Scalar(250, 250, 250),
                   cv::FILLED, cv::LINE_AA);
    }
}

void draw_rect(cv::Mat& image, float cx, float cy, float width, float height,
               float angle, const cv::Scalar& color) {
    const cv::RotatedRect rect(cv::Point2f(cx, cy), cv::Size2f(width, height),
                               angle);
    cv::Point2f points[4];
    rect.points(points);
    std::vector<cv::Point> polygon;
    for (const cv::Point2f& point : points) {
        polygon.emplace_back(cvRound(point.x), cvRound(point.y));
    }
    cv::fillConvexPoly(image, polygon, color, cv::LINE_AA);
}

const cv::Scalar GREEN(90, 185, 125);
const cv::Scalar WHITE(245, 245, 245);
const cv::Scalar RED(40, 40, 210);
const cv::Scalar DARK(55, 60, 65);
const cv::Scalar BLUE(210, 80, 40);

cv::Mat scene_balls_1() {
    cv::Mat image = make_background(900, 1000, cv::Scalar(205, 210, 215));
    const int xs[] = {100, 240, 380, 520, 660, 800};
    const int ys[] = {140, 470, 800};
    int index = 0;
    for (int y : ys) {
        for (int x : xs) {
            const cv::Scalar palette[] = {
                cv::Scalar(90, 180, 120), cv::Scalar(110, 200, 140),
                cv::Scalar(70, 160, 100), cv::Scalar(120, 210, 150),
                cv::Scalar(100, 190, 125)};
            draw_ball(image, x, y, 42, palette[index % 5]);
            ++index;
        }
    }
    draw_ball(image, 700, 950, 45, WHITE);
    draw_rect(image, 60, 650, 60, 90, 0, RED);
    draw_rect(image, 860, 260, 60, 90, 0, RED);
    return image;
}

cv::Mat scene_balls_2() {
    cv::Mat image = make_background(900, 1000, cv::Scalar(125, 130, 135));
    const int xs[] = {120, 320, 520, 720};
    const int ys[] = {130, 370, 610};
    const cv::Scalar palette[] = {
        cv::Scalar(60, 130, 85), cv::Scalar(75, 145, 100),
        cv::Scalar(50, 115, 75), cv::Scalar(80, 150, 105)};
    int index = 0;
    for (int y : ys) {
        for (int x : xs) {
            draw_ball(image, x, y, 42, palette[index % 4]);
            ++index;
        }
    }
    draw_ball(image, 200, 900, 40, GREEN);
    draw_ball(image, 600, 900, 40, GREEN);
    draw_ball(image, 450, 900, 45, WHITE);
    draw_rect(image, 820, 880, 40, 160, 20, GREEN);
    draw_rect(image, 30, 300, 50, 50, 0, RED);
    draw_rect(image, 850, 300, 50, 50, 0, RED);
    return image;
}

cv::Mat scene_balls_3() {
    cv::Mat image = make_background(900, 1000, cv::Scalar(235, 240, 245));
    const int xs[] = {100, 270, 440, 610, 780};
    const int ys[] = {130, 370, 610, 850};
    int index = 0;
    for (int y : ys) {
        for (int x : xs) {
            const cv::Scalar palette[] = {
                cv::Scalar(120, 220, 155), cv::Scalar(140, 235, 170),
                cv::Scalar(105, 205, 140), cv::Scalar(130, 230, 160)};
            draw_ball(image, x, y, 40, palette[index % 4], index % 3 == 0);
            ++index;
        }
    }
    draw_ball(image, 185, 950, 40, WHITE);
    draw_ball(image, 695, 950, 40, WHITE);
    draw_rect(image, 30, 500, 40, 140, 0, RED);
    return image;
}

cv::Mat scene_balls_4() {
    cv::Mat image = make_background(900, 1000, cv::Scalar(180, 185, 190));
    const int xs[] = {100, 300, 500, 700};
    const int ys[] = {150, 450, 750};
    const cv::Scalar palette[] = {
        cv::Scalar(85, 175, 115), cv::Scalar(100, 190, 130),
        cv::Scalar(75, 165, 105)};
    int index = 0;
    for (int y : ys) {
        for (int x : xs) {
            draw_ball(image, x, y, 38, palette[index % 3]);
            ++index;
        }
    }
    draw_ball(image, 180, 950, 38, GREEN);
    draw_ball(image, 250, 950, 38, GREEN);
    draw_ball(image, 600, 950, 38, GREEN);
    draw_ball(image, 670, 950, 38, GREEN);
    draw_ball(image, 420, 930, 40, WHITE);
    draw_rect(image, 20, 930, 50, 50, 0, RED);
    draw_rect(image, 830, 900, 35, 130, 20, GREEN);
    return image;
}

cv::Mat scene_blocks() {
    cv::Mat image = make_background(900, 700, cv::Scalar(225, 230, 235));
    const cv::Scalar shades[] = {
        cv::Scalar(60, 60, 60), cv::Scalar(50, 70, 90),
        cv::Scalar(80, 55, 45), cv::Scalar(45, 80, 70)};
    draw_rect(image, 140, 140, 90, 90, 0, shades[0]);
    draw_rect(image, 380, 140, 100, 100, 15, shades[1]);
    draw_rect(image, 650, 150, 85, 85, 30, shades[2]);
    draw_rect(image, 200, 380, 95, 95, 45, shades[3]);
    draw_rect(image, 450, 380, 70, 150, 10, shades[0]);
    draw_rect(image, 720, 400, 80, 140, -20, shades[1]);
    cv::circle(image, cv::Point(160, 600), 55, shades[2], cv::FILLED,
               cv::LINE_AA);
    draw_rect(image, 430, 600, 60, 130, 35, shades[3]);
    draw_rect(image, 700, 600, 90, 160, -10, shades[0]);
    return image;
}

cv::Mat scene_touching() {
    cv::Mat image = make_background(800, 600, cv::Scalar(225, 230, 235));
    const int pairs[][4] = {
        {180, 150, 250, 150}, {500, 160, 570, 160},
        {220, 340, 290, 340}, {520, 360, 590, 360},
        {380, 500, 450, 500}};
    for (const auto& pair : pairs) {
        draw_ball(image, pair[0], pair[1], 40, GREEN);
        draw_ball(image, pair[2], pair[3], 40, GREEN);
    }
    draw_ball(image, 100, 480, 40, GREEN);
    draw_ball(image, 700, 480, 40, GREEN);
    return image;
}

cv::Mat scene_card() {
    cv::Mat card(320, 500, CV_8UC3, cv::Scalar(245, 245, 245));
    const cv::Scalar border(25, 25, 25);
    cv::rectangle(card, cv::Rect(0, 0, 500, 12), border, cv::FILLED);
    cv::rectangle(card, cv::Rect(0, 308, 500, 12), border, cv::FILLED);
    cv::rectangle(card, cv::Rect(0, 0, 12, 320), border, cv::FILLED);
    cv::rectangle(card, cv::Rect(488, 0, 12, 320), border, cv::FILLED);
    cv::rectangle(card, cv::Rect(12, 12, 36, 36), BLUE, cv::FILLED);
    cv::rectangle(card, cv::Rect(452, 12, 36, 36), cv::Scalar(40, 210, 40),
                  cv::FILLED);
    cv::rectangle(card, cv::Rect(452, 272, 36, 36), RED, cv::FILLED);
    cv::rectangle(card, cv::Rect(12, 272, 36, 36), cv::Scalar(40, 220, 220),
                  cv::FILLED);
    const std::string label = "CARD";
    int baseline = 0;
    const cv::Size text = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX,
                                          1.4, 3, &baseline);
    cv::putText(card, label,
                cv::Point((500 - text.width) / 2, (320 + text.height) / 2),
                cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(25, 25, 25), 3,
                cv::LINE_AA);

    cv::Mat image = make_background(900, 650, cv::Scalar(200, 205, 210));
    const cv::Point2f source[4] = {
        {0.0F, 0.0F}, {499.0F, 0.0F}, {499.0F, 319.0F}, {0.0F, 319.0F}};
    const cv::Point2f target[4] = {
        {180.0F, 120.0F}, {720.0F, 180.0F}, {760.0F, 540.0F},
        {140.0F, 500.0F}};
    const cv::Mat transform = cv::getPerspectiveTransform(source, target);
    cv::warpPerspective(card, image, transform, image.size(), cv::INTER_LINEAR,
                        cv::BORDER_CONSTANT, cv::Scalar(200, 205, 210));
    return image;
}

cv::Mat match_template() {
    cv::Mat image(60, 60, CV_8UC3, cv::Scalar(245, 245, 245));
    cv::rectangle(image, cv::Rect(6, 6, 48, 48), cv::Scalar(40, 40, 220),
                  cv::FILLED);
    cv::line(image, cv::Point(6, 30), cv::Point(53, 30), WHITE, 4,
             cv::LINE_AA);
    cv::line(image, cv::Point(30, 6), cv::Point(30, 53), WHITE, 4,
             cv::LINE_AA);
    return image;
}

cv::Mat match_scene(const cv::Mat& templ) {
    cv::Mat image = make_background(800, 600, cv::Scalar(230, 234, 238));
    templ.copyTo(image(cv::Rect(100, 90, templ.cols, templ.rows)));
    cv::Mat small;
    cv::resize(templ, small, cv::Size(30, 30), 0, 0, cv::INTER_AREA);
    small.copyTo(image(cv::Rect(520, 380, small.cols, small.rows)));
    cv::rectangle(image, cv::Rect(620, 100, 60, 60), BLUE, cv::FILLED);
    cv::circle(image, cv::Point(300, 300), 35, cv::Scalar(160, 160, 160),
               cv::FILLED, cv::LINE_AA);
    cv::circle(image, cv::Point(700, 500), 30, cv::Scalar(180, 180, 180),
               cv::FILLED, cv::LINE_AA);
    return image;
}

bool save(const std::string& path, const cv::Mat& image) {
    const bool ok = cv::imwrite(path, image);
    std::cout << "saved " << path << ": " << (ok ? "yes" : "no") << '\n';
    return ok;
}

}  // namespace

int main() {
    const std::string dir = "../素材/";
    bool ok = true;
    ok = save(dir + "synth_balls_1.png", scene_balls_1()) && ok;
    ok = save(dir + "synth_balls_2.png", scene_balls_2()) && ok;
    ok = save(dir + "synth_balls_3.png", scene_balls_3()) && ok;
    ok = save(dir + "synth_balls_4.png", scene_balls_4()) && ok;
    ok = save(dir + "synth_blocks.png", scene_blocks()) && ok;
    ok = save(dir + "synth_touching.png", scene_touching()) && ok;
    ok = save(dir + "synth_card.png", scene_card()) && ok;
    const cv::Mat templ = match_template();
    ok = save(dir + "synth_match_template.png", templ) && ok;
    ok = save(dir + "synth_match_scene.png", match_scene(templ)) && ok;
    return ok ? 0 : 1;
}
