#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

namespace {

void label(cv::Mat& panel, const std::string& text) {
    cv::putText(panel, text, cv::Point(10, 26), cv::FONT_HERSHEY_SIMPLEX,
                0.65, cv::Scalar(255, 255, 255), 3, cv::LINE_AA);
    cv::putText(panel, text, cv::Point(10, 26), cv::FONT_HERSHEY_SIMPLEX,
                0.65, cv::Scalar(30, 30, 30), 1, cv::LINE_AA);
}

}  // namespace

int main() {
    const cv::Mat source = cv::imread("../素材/synth_distort.png");
    if (source.empty()) {
        std::cerr << "无法读取 synth_distort.png\n";
        return 1;
    }
    const cv::Size size = source.size();
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 500.0,
        0.0, 800.0, 300.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = (cv::Mat_<double>(1, 5) <<
        -0.35, 0.12, 0.001, -0.0005, 0.0);

    cv::Rect roi0;
    cv::Rect roi1;
    const cv::Mat new_k0 = cv::getOptimalNewCameraMatrix(
        K, D, size, 0.0, size, &roi0);
    const cv::Mat new_k1 = cv::getOptimalNewCameraMatrix(
        K, D, size, 1.0, size, &roi1);
    cv::Mat undistorted0;
    cv::Mat undistorted1;
    cv::undistort(source, undistorted0, K, D, new_k0);
    cv::undistort(source, undistorted1, K, D, new_k1);

    cv::Mat map1;
    cv::Mat map2;
    cv::initUndistortRectifyMap(K, D, cv::Mat(), new_k0, size,
                                CV_16SC2, map1, map2);
    cv::Mat remapped;
    cv::remap(source, remapped, map1, map2, cv::INTER_LINEAR);
    const double remap_diff = cv::norm(undistorted0, remapped,
                                       cv::NORM_INF);

    cv::Mat canvas(300, 1500, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat panels[3];
    for (int i = 0; i < 3; ++i) {
        panels[i] = canvas(cv::Rect(i * 500, 0, 500, 300));
    }
    cv::resize(source, panels[0], panels[0].size());
    cv::resize(undistorted0, panels[1], panels[1].size());
    cv::resize(undistorted1, panels[2], panels[2].size());
    label(panels[0], "distorted");
    label(panels[1], "alpha=0");
    label(panels[2], "alpha=1");
    const bool saved = cv::imwrite("../素材/out_undistort.png", canvas);

    std::vector<cv::Point2f> before = {cv::Point2f(200.0f, 150.0f)};
    std::vector<cv::Point2f> after;
    cv::undistortPoints(before, after, K, D, cv::noArray(), new_k0);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "image = " << size.width << "x" << size.height << '\n';
    std::cout << "K_new alpha=0 =\n" << new_k0 << '\n';
    std::cout << "roi alpha=0 = (" << roi0.x << ", " << roi0.y << ", "
              << roi0.width << ", " << roi0.height << ")\n";
    std::cout << "K_new alpha=1 =\n" << new_k1 << '\n';
    std::cout << "roi alpha=1 = (" << roi1.x << ", " << roi1.y << ", "
              << roi1.width << ", " << roi1.height << ")\n";
    std::cout << "map1 = " << map1.cols << "x" << map1.rows
              << " type=" << map1.type() << " channels="
              << map1.channels() << '\n';
    std::cout << std::setprecision(6);
    std::cout << "undistort vs remap max diff = " << remap_diff << '\n';
    std::cout << std::setprecision(3);
    std::cout << "point (200, 150) -> (" << after[0].x << ", "
              << after[0].y << ")\n";
    std::cout << "saved out_undistort.png 1500x300: "
              << (saved ? "yes" : "no") << '\n';
    return 0;
}
