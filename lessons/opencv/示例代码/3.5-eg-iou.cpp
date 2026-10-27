#include <algorithm>
#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>

double iou(const cv::Rect& a, const cv::Rect& b) {
    const int left = std::max(a.x, b.x);
    const int top = std::max(a.y, b.y);
    const int right = std::min(a.x + a.width, b.x + b.width);
    const int bottom = std::min(a.y + a.height, b.y + b.height);
    const int overlap = std::max(0, right - left) * std::max(0, bottom - top);
    const int area = a.area() + b.area() - overlap;
    return area > 0 ? static_cast<double>(overlap) / area : 0.0;
}

int main() {
    const cv::Rect first(5, 5, 10, 10);
    const cv::Rect overlap(8, 5, 10, 10);
    const cv::Rect apart(20, 5, 10, 10);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "iou overlap=" << iou(first, overlap) << '\n';
    std::cout << "iou apart=" << iou(first, apart) << '\n';
    std::cout << "suppress overlap="
              << (iou(first, overlap) > 0.40 ? "yes" : "no") << '\n';
    std::cout << "suppress apart="
              << (iou(first, apart) > 0.40 ? "yes" : "no") << '\n';
    return 0;
}
