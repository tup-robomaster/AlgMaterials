#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const cv::Mat K_true = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 450.0,
        0.0, 780.0, 350.0,
        0.0, 0.0, 1.0);
    const cv::Mat D_true = (cv::Mat_<double>(1, 5) <<
        -0.25, 0.08, 0.0, 0.0, 0.0);

    std::vector<cv::Point3f> object;
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 9; ++col) {
            object.push_back(cv::Point3f(
                static_cast<float>(col * 0.04),
                static_cast<float>(row * 0.04), 0.0f));
        }
    }
    const cv::Vec3d rvecs_true[6] = {
        {0.15, -0.10, 0.02}, {0.25, -0.20, 0.05},
        {-0.18, 0.22, -0.04}, {0.30, 0.10, 0.10},
        {-0.10, -0.28, 0.06}, {0.12, 0.30, -0.12},
    };
    const cv::Vec3d tvecs_true[6] = {
        {-0.15, -0.10, 0.85}, {-0.12, -0.08, 0.90},
        {-0.10, -0.06, 0.95}, {-0.20, -0.12, 0.80},
        {-0.08, -0.04, 1.00}, {-0.16, -0.10, 0.88},
    };

    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;
    for (int i = 0; i < 6; ++i) {
        std::vector<cv::Point2f> projected;
        cv::projectPoints(object, rvecs_true[i], tvecs_true[i], K_true,
                          D_true, projected);
        object_points.push_back(object);
        image_points.push_back(projected);
    }

    cv::Mat K = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    std::vector<cv::Vec3d> rvecs;
    std::vector<cv::Vec3d> tvecs;
    const double rms = cv::calibrateCamera(
        object_points, image_points, cv::Size(900, 700), K, D, rvecs,
        tvecs, cv::CALIB_FIX_K3 | cv::CALIB_ZERO_TANGENT_DIST);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "rms = " << rms << '\n';
    std::cout << "fx = " << K.at<double>(0, 0) << '\n';
    std::cout << "fy = " << K.at<double>(1, 1) << '\n';
    std::cout << "cx = " << K.at<double>(0, 2) << '\n';
    std::cout << "cy = " << K.at<double>(1, 2) << '\n';
    std::cout << "k1 = " << D.at<double>(0, 0) << '\n';
    std::cout << "k2 = " << D.at<double>(0, 1) << '\n';
    return 0;
}
