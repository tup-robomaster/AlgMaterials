#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>

namespace {

struct Bar {
    std::string name;
    cv::Point2f center;
    double angle = 0.0;
    double length = 0.0;
};

double angleDifference(double a, double b) {
    double diff = std::fabs(a - b);
    if (diff > 90.0) {
        diff = 180.0 - diff;
    }
    return diff;
}

void check(const Bar& a, const Bar& b) {
    const double angle_diff = angleDifference(a.angle, b.angle);
    const double y_diff = std::fabs(a.center.y - b.center.y);
    const double distance = cv::norm(a.center - b.center);
    const double length_ratio = std::max(a.length, b.length) /
                                std::min(a.length, b.length);
    const bool accepted = angle_diff <= 20.0 && y_diff <= 25.0 &&
                          distance >= 80.0 && distance <= 220.0 &&
                          length_ratio <= 1.25;
    std::cout << a.name << " " << b.name
              << ": angle_diff=" << cvRound(angle_diff)
              << " y_diff=" << cvRound(y_diff)
              << " distance=" << cvRound(distance)
              << " ratio=" << cvRound(length_ratio * 100.0) / 100.0
              << " accepted=" << (accepted ? "yes" : "no") << '\n';
}

}  // namespace

int main() {
    const Bar a{"A", cv::Point2f(330, 360), 8.0, 170.0};
    const Bar b{"B", cv::Point2f(470, 360), 12.0, 170.0};
    const Bar c{"C", cv::Point2f(480, 360), 100.0, 160.0};
    const Bar d{"D", cv::Point2f(100, 100), 178.0, 150.0};
    const Bar e{"E", cv::Point2f(240, 100), 2.0, 150.0};

    check(a, b);
    check(a, c);
    check(d, e);
    return 0;
}
