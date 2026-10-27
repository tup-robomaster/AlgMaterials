#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

int main() {
    const cv::Mat camera = (cv::Mat_<double>(3, 3) << 800.0, 0.0, 320.0,
                            0.0, 800.0, 240.0, 0.0, 0.0, 1.0);
    const cv::Mat distortion =
        (cv::Mat_<double>(1, 5) << -0.25, 0.08, 0.001, -0.0005, 0.0);
    const std::vector<cv::Point2f> pixels = {cv::Point2f(420.0F, 180.0F)};

    std::vector<cv::Point2f> normalized;
    cv::undistortPoints(pixels, normalized, camera, distortion);
    std::vector<cv::Point2f> mapped;
    cv::undistortPoints(pixels, mapped, camera, distortion, cv::noArray(),
                        camera);

    const std::vector<cv::Point3d> ray = {
        cv::Point3d(normalized[0].x, normalized[0].y, 1.0)};
    const std::vector<cv::Point3d> point = {cv::Point3d(0.2, -0.1, 2.0)};
    const cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
    const cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);
    std::vector<cv::Point2d> ray_pixel;
    std::vector<cv::Point2d> point_pixel;
    cv::projectPoints(ray, rvec, tvec, camera, distortion, ray_pixel);
    cv::projectPoints(point, rvec, tvec, camera, distortion, point_pixel);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "normalized = (" << normalized[0].x << ", "
              << normalized[0].y << ")\n";
    std::cout << "mapped pixel = (" << mapped[0].x << ", " << mapped[0].y
              << ")\n";
    std::cout << "project normalized -> (" << ray_pixel[0].x << ", "
              << ray_pixel[0].y << ")\n";
    std::cout << "reprojection error = "
              << std::hypot(ray_pixel[0].x - pixels[0].x,
                            ray_pixel[0].y - pixels[0].y)
              << " px\n";
    std::cout << "project (0.2, -0.1, 2.0) -> (" << point_pixel[0].x << ", "
              << point_pixel[0].y << ")\n";
    return 0;
}
