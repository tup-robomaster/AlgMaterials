#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    std::vector<cv::Point3f> cloud;
    for (int row = 0; row < 25; ++row) {
        for (int col = 0; col < 25; ++col) {
            const double x = (col - 12) * 0.04;
            const double y = (row - 12) * 0.04;
            const double z = 1.2 + 0.15 * std::sin(0.4 * col) *
                                     std::cos(0.3 * row);
            cloud.push_back(cv::Point3f(
                static_cast<float>(x), static_cast<float>(y),
                static_cast<float>(z)));
        }
    }
    for (int i = 0; i < 10; ++i) {
        cloud.push_back(cv::Point3f(
            static_cast<float>(-0.4 + 0.08 * i),
            static_cast<float>(0.1 * i), -0.5f));
    }

    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        600.0, 0.0, 450.0,
        0.0, 600.0, 300.0,
        0.0, 0.0, 1.0);
    const cv::Vec3d rvec(0.15, -0.20, 0.05);
    const cv::Vec3d tvec(0.0, 0.0, 0.0);
    cv::Mat R;
    cv::Rodrigues(rvec, R);
    const double fx = K.at<double>(0, 0);
    const double fy = K.at<double>(1, 1);
    const double cx = K.at<double>(0, 2);
    const double cy = K.at<double>(1, 2);

    int invalid_z = 0;
    int outside = 0;
    int drawn = 0;
    double depth_sum = 0.0;
    double max_diff = 0.0;
    std::vector<cv::Point3f> valid_points;
    std::vector<cv::Point2f> manual_pixels;
    cv::Mat image(600, 900, CV_8UC3, cv::Scalar(30, 30, 30));

    for (const cv::Point3f& p : cloud) {
        const double x = R.at<double>(0, 0) * p.x +
                         R.at<double>(0, 1) * p.y +
                         R.at<double>(0, 2) * p.z + tvec[0];
        const double y = R.at<double>(1, 0) * p.x +
                         R.at<double>(1, 1) * p.y +
                         R.at<double>(1, 2) * p.z + tvec[1];
        const double z = R.at<double>(2, 0) * p.x +
                         R.at<double>(2, 1) * p.y +
                         R.at<double>(2, 2) * p.z + tvec[2];
        if (z <= 0.0) {
            ++invalid_z;
            continue;
        }
        const double u = fx * x / z + cx;
        const double v = fy * y / z + cy;
        if (u < 0.0 || u >= image.cols || v < 0.0 ||
            v >= image.rows) {
            ++outside;
            continue;
        }
        valid_points.push_back(p);
        manual_pixels.push_back(cv::Point2f(
            static_cast<float>(u), static_cast<float>(v)));
        depth_sum += z;
        const double t = std::min(1.0, std::max(0.0, (z - 1.0) / 0.6));
        const cv::Scalar color(255.0 * (1.0 - t), 80, 255.0 * t);
        cv::circle(image, cv::Point(cvRound(u), cvRound(v)), 2, color,
                   cv::FILLED, cv::LINE_AA);
        ++drawn;
    }

    std::vector<cv::Point2f> projected;
    cv::projectPoints(valid_points, rvec, tvec, K,
                      cv::Mat::zeros(1, 5, CV_64F), projected);
    for (std::size_t i = 0; i < projected.size(); ++i) {
        const cv::Point2f diff = projected[i] - manual_pixels[i];
        max_diff = std::max(
            max_diff, std::sqrt(static_cast<double>(diff.x) * diff.x +
                                static_cast<double>(diff.y) * diff.y));
    }

    const bool saved = cv::imwrite("../素材/out_pointcloud.png", image);
    const double mean_depth = drawn > 0 ? depth_sum / drawn : 0.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "points = " << cloud.size() << '\n';
    std::cout << "invalid_z = " << invalid_z << '\n';
    std::cout << "outside = " << outside << '\n';
    std::cout << "drawn = " << drawn << '\n';
    std::cout << std::setprecision(3);
    std::cout << "mean_depth = " << mean_depth << '\n';
    std::cout << std::setprecision(6);
    std::cout << "max_pixel_diff = " << max_diff << '\n';
    std::cout << "saved out_pointcloud.png " << image.cols << "x"
              << image.rows << ": " << (saved ? "yes" : "no") << '\n';
    return 0;
}
