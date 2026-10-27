// 生成第 7 部分用的合成素材：
//   synth_chess_1.png  900×700，9×6 内角点的棋盘，姿态一
//   synth_chess_2.png  900×700，同一棋盘，姿态二
//   synth_chess_3.png  900×700，同一棋盘，姿态三
//   synth_distort.png  1000×600，带径向与切向畸变的网格
// 位姿、内参、畸变系数都写死，重复运行得到同一组画面。

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

namespace {

constexpr int kCols = 9;
constexpr int kRows = 6;
constexpr double kSquare = 0.04;
constexpr int kCell = 40;
constexpr int kMargin = 20;

cv::Mat makeBoardTexture() {
    const int width = (kCols + 1) * kCell + 2 * kMargin;
    const int height = (kRows + 1) * kCell + 2 * kMargin;
    cv::Mat board(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    for (int row = 0; row < kRows + 1; ++row) {
        for (int col = 0; col < kCols + 1; ++col) {
            if ((row + col) % 2 != 0) {
                continue;
            }
            const int x = kMargin + col * kCell;
            const int y = kMargin + row * kCell;
            cv::rectangle(board, cv::Rect(x, y, kCell, kCell),
                          cv::Scalar(20, 20, 20), cv::FILLED);
        }
    }
    return board;
}

cv::Mat renderBoard(const cv::Mat& texture, const cv::Mat& K,
                    const cv::Vec3d& rvec, const cv::Vec3d& tvec,
                    const cv::Size& size) {
    const double px = kSquare / kCell;
    const std::vector<cv::Point3f> corners = {
        cv::Point3f(static_cast<float>(-kMargin * px),
                    static_cast<float>(-kMargin * px), 0.0f),
        cv::Point3f(static_cast<float>((texture.cols - kMargin) * px),
                    static_cast<float>(-kMargin * px), 0.0f),
        cv::Point3f(static_cast<float>((texture.cols - kMargin) * px),
                    static_cast<float>((texture.rows - kMargin) * px),
                    0.0f),
        cv::Point3f(static_cast<float>(-kMargin * px),
                    static_cast<float>((texture.rows - kMargin) * px),
                    0.0f),
    };

    std::vector<cv::Point2f> image_corners;
    cv::projectPoints(corners, rvec, tvec, K,
                      cv::Mat::zeros(1, 5, CV_64F), image_corners);

    const std::vector<cv::Point2f> texture_corners = {
        cv::Point2f(0.0f, 0.0f),
        cv::Point2f(static_cast<float>(texture.cols), 0.0f),
        cv::Point2f(static_cast<float>(texture.cols),
                    static_cast<float>(texture.rows)),
        cv::Point2f(0.0f, static_cast<float>(texture.rows)),
    };
    const cv::Mat H = cv::getPerspectiveTransform(texture_corners,
                                                   image_corners);

    cv::Mat view(size, CV_8UC3, cv::Scalar(70, 70, 70));
    cv::Mat warped;
    cv::warpPerspective(texture, warped, H, size, cv::INTER_LINEAR,
                        cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    cv::Mat mask(size, CV_8UC1, cv::Scalar(0));
    std::vector<cv::Point> polygon;
    for (const cv::Point2f& p : image_corners) {
        polygon.push_back(cv::Point(cvRound(p.x), cvRound(p.y)));
    }
    cv::fillConvexPoly(mask, polygon, cv::Scalar(255));
    warped.copyTo(view, mask);
    return view;
}

void drawLine(std::vector<cv::Point>& points, cv::Mat& image,
              const cv::Mat& K, const cv::Mat& D, double x0, double y0,
              double x1, double y1) {
    points.clear();
    const int steps = 80;
    for (int i = 0; i <= steps; ++i) {
        const double t = static_cast<double>(i) / steps;
        const double x = x0 + (x1 - x0) * t;
        const double y = y0 + (y1 - y0) * t;
        std::vector<cv::Point3f> source = {cv::Point3f(
            static_cast<float>(x), static_cast<float>(y), 0.0f)};
        std::vector<cv::Point2f> distorted;
        cv::projectPoints(source, cv::Vec3d(0, 0, 0), cv::Vec3d(0, 0, 0),
                          K, D, distorted);
        points.push_back(cv::Point(cvRound(distorted[0].x),
                                   cvRound(distorted[0].y)));
    }
    cv::polylines(image, points, false, cv::Scalar(40, 40, 200), 2,
                  cv::LINE_AA);
}

cv::Mat makeDistortionGrid() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 500.0,
        0.0, 800.0, 300.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = (cv::Mat_<double>(1, 5) <<
        -0.35, 0.12, 0.001, -0.0005, 0.0);
    cv::Mat image(600, 1000, CV_8UC3, cv::Scalar(245, 245, 245));

    std::vector<cv::Point> points;
    for (int i = -6; i <= 6; ++i) {
        const double x = i * 0.1;
        drawLine(points, image, K, D, x, -0.35, x, 0.35);
    }
    for (int j = -3; j <= 3; ++j) {
        const double y = j * 0.1;
        drawLine(points, image, K, D, -0.6, y, 0.6, y);
    }
    cv::circle(image, cv::Point(500, 300), 4, cv::Scalar(30, 30, 30),
               cv::FILLED, cv::LINE_AA);
    return image;
}

}  // namespace

int main() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 450.0,
        0.0, 800.0, 350.0,
        0.0, 0.0, 1.0);
    const cv::Mat board = makeBoardTexture();
    const cv::Size size(900, 700);

    struct Pose {
        const char* name;
        cv::Vec3d rvec;
        cv::Vec3d tvec;
    };
    const Pose poses[] = {
        {"synth_chess_1.png", cv::Vec3d(0.20, -0.15, 0.03),
         cv::Vec3d(-0.18, -0.12, 0.80)},
        {"synth_chess_2.png", cv::Vec3d(0.25, -0.20, 0.05),
         cv::Vec3d(-0.22, -0.12, 0.82)},
        {"synth_chess_3.png", cv::Vec3d(-0.22, 0.28, -0.06),
         cv::Vec3d(-0.16, -0.10, 0.95)},
    };

    for (const Pose& pose : poses) {
        const cv::Mat view = renderBoard(board, K, pose.rvec, pose.tvec,
                                         size);
        const std::string path = std::string("../素材/") + pose.name;
        if (!cv::imwrite(path, view)) {
            std::cerr << "无法写入 " << path << '\n';
            return 1;
        }
        std::cout << "saved " << pose.name << " " << view.cols << "x"
                  << view.rows << '\n';
    }

    const cv::Mat grid = makeDistortionGrid();
    if (!cv::imwrite("../素材/synth_distort.png", grid)) {
        std::cerr << "无法写入 synth_distort.png\n";
        return 1;
    }
    std::cout << "saved synth_distort.png " << grid.cols << "x"
              << grid.rows << '\n';
    return 0;
}
