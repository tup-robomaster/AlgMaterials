#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
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

void printDistortion(const char* name, const cv::Mat& D) {
    std::cout << name << " = [";
    for (int i = 0; i < 5; ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << D.at<double>(0, i);
    }
    std::cout << "]\n";
}

}  // namespace

int main() {
    const cv::Mat K_true = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 450.0,
        0.0, 780.0, 350.0,
        0.0, 0.0, 1.0);
    const cv::Mat D_true = (cv::Mat_<double>(1, 5) <<
        -0.25, 0.08, 0.0, 0.0, 0.0);
    const cv::Size image_size(900, 700);

    struct Pose {
        cv::Vec3d rvec;
        cv::Vec3d tvec;
    };
    const Pose poses[] = {
        {cv::Vec3d(0.15, -0.10, 0.02), cv::Vec3d(-0.15, -0.10, 0.85)},
        {cv::Vec3d(0.25, -0.20, 0.05), cv::Vec3d(-0.12, -0.08, 0.90)},
        {cv::Vec3d(-0.18, 0.22, -0.04), cv::Vec3d(-0.10, -0.06, 0.95)},
        {cv::Vec3d(0.30, 0.10, 0.10), cv::Vec3d(-0.20, -0.12, 0.80)},
        {cv::Vec3d(-0.10, -0.28, 0.06), cv::Vec3d(-0.08, -0.04, 1.00)},
        {cv::Vec3d(0.12, 0.30, -0.12), cv::Vec3d(-0.16, -0.10, 0.88)},
        {cv::Vec3d(-0.24, 0.08, 0.08), cv::Vec3d(-0.14, -0.09, 0.92)},
        {cv::Vec3d(0.20, -0.25, -0.06), cv::Vec3d(-0.18, -0.11, 0.86)},
    };

    constexpr std::size_t view_count = sizeof(poses) / sizeof(poses[0]);
    const std::vector<cv::Point3f> object = boardPoints();
    const std::vector<cv::Point3f> object_const = object;
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;
    for (const Pose& pose : poses) {
        std::vector<cv::Point2f> projected;
        cv::projectPoints(object_const, pose.rvec, pose.tvec, K_true,
                          D_true, projected);
        object_points.push_back(object_const);
        image_points.push_back(projected);
    }

    cv::Mat K = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    std::vector<cv::Vec3d> rvecs;
    std::vector<cv::Vec3d> tvecs;
    const double rms = cv::calibrateCamera(
        object_points, image_points, image_size, K, D, rvecs, tvecs,
        cv::CALIB_FIX_K3 | cv::CALIB_ZERO_TANGENT_DIST,
        cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS,
                         100, 1e-9));

    double max_view_error = 0.0;
    for (std::size_t i = 0; i < view_count; ++i) {
        std::vector<cv::Point2f> projected;
        cv::projectPoints(object_points[i], rvecs[i], tvecs[i], K, D,
                          projected);
        double sum = 0.0;
        for (std::size_t j = 0; j < projected.size(); ++j) {
            const cv::Point2f diff = projected[j] - image_points[i][j];
            sum += diff.x * diff.x + diff.y * diff.y;
        }
        max_view_error = std::max(
            max_view_error, std::sqrt(sum / projected.size()));
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "views = " << view_count << " image = "
              << image_size.width << "x" << image_size.height << '\n';
    std::cout << "K_true =\n" << K_true << '\n';
    std::cout << "K_est =\n" << K << '\n';
    printDistortion("D_true", D_true);
    printDistortion("D_est", D);
    std::cout << std::setprecision(8);
    std::cout << "rms = " << rms << '\n';
    std::cout << std::setprecision(6);
    std::cout << "max_view_error = " << max_view_error << '\n';
    std::cout << "fx_error = " << K.at<double>(0, 0) -
                                    K_true.at<double>(0, 0) << '\n';
    std::cout << "fy_error = " << K.at<double>(1, 1) -
                                    K_true.at<double>(1, 1) << '\n';
    std::cout << "cx_error = " << K.at<double>(0, 2) -
                                    K_true.at<double>(0, 2) << '\n';
    std::cout << "cy_error = " << K.at<double>(1, 2) -
                                    K_true.at<double>(1, 2) << '\n';
    return 0;
}
