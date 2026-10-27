// 生成第 5 部分用的合成视频：
//   cube.mp4   640×480、30 fps、90 帧，一个旋转的线框立方体。
// 位置、角度与颜色都写死，重复运行得到同一段画面。

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

struct Point3 {
    double x;
    double y;
    double z;
};

Point3 rotatePoint(const Point3& p, double ax, double ay) {
    const double cy = std::cos(ay);
    const double sy = std::sin(ay);
    const double cx = std::cos(ax);
    const double sx = std::sin(ax);

    const Point3 after_y{p.x * cy + p.z * sy, p.y,
                         -p.x * sy + p.z * cy};
    return Point3{after_y.x, after_y.y * cx - after_y.z * sx,
                  after_y.y * sx + after_y.z * cx};
}

cv::Point project(const Point3& p) {
    const double distance = 3.6;
    const double scale = 420.0 / (distance - p.z);
    return cv::Point(cvRound(320.0 + p.x * scale),
                     cvRound(240.0 + p.y * scale));
}

void drawBackground(cv::Mat& frame) {
    for (int y = 0; y < frame.rows; ++y) {
        const int value = 24 + y * 60 / frame.rows;
        frame.row(y).setTo(cv::Scalar(value, value, value + 4));
    }
    cv::circle(frame, cv::Point(520, 90), 58, cv::Scalar(70, 60, 45),
               cv::FILLED, cv::LINE_AA);
    cv::circle(frame, cv::Point(80, 400), 90, cv::Scalar(45, 55, 70),
               cv::FILLED, cv::LINE_AA);
}

}  // namespace

int main() {
    const std::string path = "../素材/cube.mp4";
    cv::VideoWriter writer(path,
                           cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                           30.0, cv::Size(640, 480));
    if (!writer.isOpened()) {
        std::cout << "无法创建 " << path << '\n';
        return 1;
    }

    const std::vector<Point3> vertices = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}};
    const int edges[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5},
                              {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5},
                              {2, 6}, {3, 7}};

    const int frames = 90;
    for (int i = 0; i < frames; ++i) {
        cv::Mat frame(480, 640, CV_8UC3);
        drawBackground(frame);

        const double ay = 0.55 + 2.0 * CV_PI * i / frames;
        const double ax = 0.45 + 0.35 * std::sin(2.0 * CV_PI * i / frames);
        std::vector<cv::Point> shown;
        std::vector<double> depth;
        for (const Point3& vertex : vertices) {
            const Point3 p = rotatePoint(vertex, ax, ay);
            shown.push_back(project(p));
            depth.push_back(p.z);
        }

        for (const auto& edge : edges) {
            const int a = edge[0];
            const int b = edge[1];
            const double mean_z = 0.5 * (depth[a] + depth[b]);
            const int green = cvRound(150.0 + 70.0 * mean_z);
            const int red = cvRound(90.0 - 60.0 * mean_z);
            cv::line(frame, shown[a], shown[b],
                     cv::Scalar(red, green, 235), 3, cv::LINE_AA);
        }
        for (std::size_t v = 0; v < shown.size(); ++v) {
            cv::circle(frame, shown[v], 4, cv::Scalar(245, 245, 245),
                       cv::FILLED, cv::LINE_AA);
        }

        writer.write(frame);
    }
    writer.release();

    cv::VideoCapture reader(path);
    std::cout << "saved " << path << ": " << (reader.isOpened() ? "yes" : "no")
              << '\n';
    if (reader.isOpened()) {
        std::cout << "width="
                  << cvRound(reader.get(cv::CAP_PROP_FRAME_WIDTH))
                  << " height="
                  << cvRound(reader.get(cv::CAP_PROP_FRAME_HEIGHT))
                  << " fps=" << reader.get(cv::CAP_PROP_FPS)
                  << " frames="
                  << cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT)) << '\n';
    }
    return 0;
}
