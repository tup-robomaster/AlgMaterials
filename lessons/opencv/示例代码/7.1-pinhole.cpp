#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

int main() {
    const cv::Mat K = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 640.0,
        0.0, 780.0, 360.0,
        0.0, 0.0, 1.0);
    const cv::Mat zero = cv::Mat::zeros(1, 5, CV_64F);
    const cv::Vec3d rvec(0.08, -0.15, 0.03);
    const cv::Vec3d tvec(0.05, -0.02, 2.50);
    const cv::Point3f world(0.20f, -0.10f, 0.0f);

    cv::Mat R;
    cv::Rodrigues(rvec, R);
    cv::Mat point = (cv::Mat_<double>(3, 1) <<
        world.x, world.y, world.z);
    cv::Mat camera = R * point;
    for (int i = 0; i < 3; ++i) {
        camera.at<double>(i, 0) += tvec[i];
    }

    const double x = camera.at<double>(0, 0);
    const double y = camera.at<double>(1, 0);
    const double z = camera.at<double>(2, 0);
    const double fx = K.at<double>(0, 0);
    const double fy = K.at<double>(1, 1);
    const double cx = K.at<double>(0, 2);
    const double cy = K.at<double>(1, 2);
    const double u = fx * x / z + cx;
    const double v = fy * y / z + cy;

    std::vector<cv::Point3f> object = {world};
    std::vector<cv::Point2f> projected;
    cv::projectPoints(object, rvec, tvec, K, zero, projected);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "K =\n" << K << '\n';
    std::cout << "rvec = [" << rvec[0] << ", " << rvec[1] << ", "
              << rvec[2] << "]\n";
    std::cout << "tvec = [" << tvec[0] << ", " << tvec[1] << ", "
              << tvec[2] << "]\n";
    std::cout << "P_world = [" << world.x << ", " << world.y << ", "
              << world.z << "]\n";
    std::cout << "P_camera = [" << x << ", " << y << ", " << z << "]\n";
    std::cout << "normalized = [" << x / z << ", " << y / z << "]\n";
    std::cout << "pixel = (" << u << ", " << v << ")\n";
    std::cout << "projectPoints = (" << projected[0].x << ", "
              << projected[0].y << ")\n";
    std::cout << "difference = (" << u - projected[0].x << ", "
              << v - projected[0].y << ")\n";

    const std::vector<cv::Point3f> axes = {
        cv::Point3f(0.0f, 0.0f, 0.0f),
        cv::Point3f(0.1f, 0.0f, 0.0f),
        cv::Point3f(0.0f, 0.1f, 0.0f),
        cv::Point3f(0.0f, 0.0f, 0.1f),
    };
    std::vector<cv::Point2f> axis_pixels;
    cv::projectPoints(axes, rvec, tvec, K, zero, axis_pixels);
    std::cout << "axis origin = (" << axis_pixels[0].x << ", "
              << axis_pixels[0].y << ")\n";
    std::cout << "axis x = (" << axis_pixels[1].x << ", "
              << axis_pixels[1].y << ")\n";
    std::cout << "axis y = (" << axis_pixels[2].x << ", "
              << axis_pixels[2].y << ")\n";
    std::cout << "axis z = (" << axis_pixels[3].x << ", "
              << axis_pixels[3].y << ")\n";
    return 0;
}
