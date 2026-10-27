#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

namespace {

std::vector<cv::Point3f> boardPoints() {
    constexpr int cols = 9;
    constexpr int rows = 6;
    constexpr double square = 0.04;
    std::vector<cv::Point3f> points;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            points.push_back(cv::Point3f(
                static_cast<float>(col * square),
                static_cast<float>(row * square), 0.0f));
        }
    }
    return points;
}

double vectorNorm(const cv::Vec3d& v) {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

}  // namespace

int main() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 450.0,
        0.0, 800.0, 350.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    const cv::Vec3d rvec_true(0.20, -0.15, 0.03);
    const cv::Vec3d tvec_true(-0.18, -0.12, 0.80);

    const std::vector<cv::Point3f> object = boardPoints();
    std::vector<cv::Point2f> image_points;
    cv::projectPoints(object, rvec_true, tvec_true, K, D, image_points);

    cv::Vec3d rvec;
    cv::Vec3d tvec;
    cv::solvePnP(object, image_points, K, D, rvec, tvec, false,
                 cv::SOLVEPNP_ITERATIVE);

    std::vector<cv::Point2f> reprojected;
    cv::projectPoints(object, rvec, tvec, K, D, reprojected);
    double sum = 0.0;
    for (std::size_t i = 0; i < reprojected.size(); ++i) {
        const cv::Point2f diff = reprojected[i] - image_points[i];
        sum += diff.x * diff.x + diff.y * diff.y;
    }
    const double rms = std::sqrt(sum / reprojected.size());

    cv::Mat image = cv::imread("../素材/synth_chess_1.png");
    if (image.empty()) {
        std::cerr << "无法读取 synth_chess_1.png\n";
        return 1;
    }

    const std::vector<cv::Point3f> axes = {
        cv::Point3f(0.0f, 0.0f, 0.0f),
        cv::Point3f(0.08f, 0.0f, 0.0f),
        cv::Point3f(0.0f, 0.08f, 0.0f),
        cv::Point3f(0.0f, 0.0f, 0.08f),
    };
    std::vector<cv::Point2f> axis_pixels;
    cv::projectPoints(axes, rvec, tvec, K, D, axis_pixels);
    cv::line(image, axis_pixels[0], axis_pixels[1], cv::Scalar(0, 0, 255),
             3, cv::LINE_AA);
    cv::line(image, axis_pixels[0], axis_pixels[2], cv::Scalar(0, 255, 0),
             3, cv::LINE_AA);
    cv::line(image, axis_pixels[0], axis_pixels[3], cv::Scalar(255, 0, 0),
             3, cv::LINE_AA);

    std::vector<cv::Point3f> cube;
    for (const double z : {0.05, 0.09}) {
        for (const double dx : {-0.02, 0.02}) {
            for (const double dy : {-0.02, 0.02}) {
                cube.push_back(cv::Point3f(
                    static_cast<float>(0.18 + dx),
                    static_cast<float>(0.12 + dy),
                    static_cast<float>(z)));
            }
        }
    }
    std::vector<cv::Point2f> cube_pixels;
    cv::projectPoints(cube, rvec, tvec, K, D, cube_pixels);
    const int edges[12][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0},
        {4, 5}, {5, 7}, {7, 6}, {6, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7},
    };
    for (const auto& edge : edges) {
        cv::line(image, cube_pixels[edge[0]], cube_pixels[edge[1]],
                 cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
    }
    const bool saved = cv::imwrite("../素材/out_pnp.png", image);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "rvec_true = [" << rvec_true[0] << ", " << rvec_true[1]
              << ", " << rvec_true[2] << "]\n";
    std::cout << "rvec_est  = [" << rvec[0] << ", " << rvec[1] << ", "
              << rvec[2] << "]\n";
    std::cout << "tvec_true = [" << tvec_true[0] << ", " << tvec_true[1]
              << ", " << tvec_true[2] << "]\n";
    std::cout << "tvec_est  = [" << tvec[0] << ", " << tvec[1] << ", "
              << tvec[2] << "]\n";
    std::cout << std::setprecision(6);
    std::cout << "rotation_diff = " << vectorNorm(rvec - rvec_true) << '\n';
    std::cout << "translation_diff = "
              << vectorNorm(tvec - tvec_true) << '\n';
    std::cout << "rms = " << rms << '\n';
    std::cout << "saved out_pnp.png " << image.cols << "x" << image.rows
              << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
