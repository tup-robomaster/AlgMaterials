#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 450.0,
        0.0, 800.0, 350.0,
        0.0, 0.0, 1.0);
    const cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    const std::vector<cv::Point3f> object = {
        cv::Point3f(0.0f, 0.0f, 0.0f),
        cv::Point3f(0.32f, 0.0f, 0.0f),
        cv::Point3f(0.32f, 0.20f, 0.0f),
        cv::Point3f(0.0f, 0.20f, 0.0f),
    };
    const cv::Vec3d rvec_true(0.20, -0.15, 0.03);
    const cv::Vec3d tvec_true(-0.18, -0.12, 0.80);
    std::vector<cv::Point2f> image_points;
    cv::projectPoints(object, rvec_true, tvec_true, K, D, image_points);

    cv::Vec3d rvec;
    cv::Vec3d tvec;
    cv::solvePnP(object, image_points, K, D, rvec, tvec, false,
                 cv::SOLVEPNP_IPPE);
    std::vector<cv::Point2f> reprojected;
    cv::projectPoints(object, rvec, tvec, K, D, reprojected);
    double sum = 0.0;
    for (std::size_t i = 0; i < reprojected.size(); ++i) {
        const cv::Point2f diff = reprojected[i] - image_points[i];
        sum += diff.x * diff.x + diff.y * diff.y;
    }
    const double rms = std::sqrt(sum / reprojected.size());

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "rvec = [" << rvec[0] << ", " << rvec[1] << ", "
              << rvec[2] << "]\n";
    std::cout << "tvec = [" << tvec[0] << ", " << tvec[1] << ", "
              << tvec[2] << "]\n";
    std::cout << std::setprecision(6);
    std::cout << "rms = " << rms << '\n';
    return 0;
}
