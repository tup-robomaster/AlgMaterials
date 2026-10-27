#include <iostream>

#include <opencv2/core.hpp>

int main() {
    cv::Mat camera_buffer(2, 4, CV_8UC3, cv::Scalar(1, 2, 3));
    cv::Mat queued = camera_buffer;
    cv::Mat snapshot = camera_buffer.clone();

    camera_buffer.setTo(cv::Scalar(9, 9, 9));

    const cv::Vec3b camera_pixel = camera_buffer.at<cv::Vec3b>(0, 0);
    const cv::Vec3b queued_pixel = queued.at<cv::Vec3b>(0, 0);
    const cv::Vec3b snapshot_pixel = snapshot.at<cv::Vec3b>(0, 0);

    std::cout << "camera=(" << static_cast<int>(camera_pixel[0]) << ","
              << static_cast<int>(camera_pixel[1]) << ","
              << static_cast<int>(camera_pixel[2]) << ")"
              << " queued=(" << static_cast<int>(queued_pixel[0]) << ","
              << static_cast<int>(queued_pixel[1]) << ","
              << static_cast<int>(queued_pixel[2]) << ")"
              << " snapshot=(" << static_cast<int>(snapshot_pixel[0]) << ","
              << static_cast<int>(snapshot_pixel[1]) << ","
              << static_cast<int>(snapshot_pixel[2]) << ")" << '\n';

    std::cout << "data camera=" << static_cast<const void*>(camera_buffer.data)
              << " queued=" << static_cast<const void*>(queued.data)
              << " snapshot=" << static_cast<const void*>(snapshot.data)
              << '\n';
    std::cout << "share queued="
              << (queued.data == camera_buffer.data ? "yes" : "no")
              << " snapshot="
              << (snapshot.data == camera_buffer.data ? "yes" : "no") << '\n';
    std::cout << "queued size=" << queued.cols << "x" << queued.rows
              << " step=" << queued.step[0] << '\n';
    return 0;
}
