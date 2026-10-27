#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

namespace {

struct Bar {
    cv::Point2f center;
    double angle = 0.0;
    double length = 0.0;
};

struct Pair {
    int first = 0;
    int second = 0;
    double angle_diff = 0.0;
    double y_diff = 0.0;
    double distance = 0.0;
    double length_ratio = 0.0;
    bool pass = false;
};

double angleDifference(double a, double b) {
    double diff = std::fabs(a - b);
    if (diff > 90.0) {
        diff = 180.0 - diff;
    }
    return diff;
}

Pair measure(const Bar& a, const Bar& b, int first, int second) {
    Pair pair;
    pair.first = first;
    pair.second = second;
    pair.angle_diff = angleDifference(a.angle, b.angle);
    pair.y_diff = std::fabs(a.center.y - b.center.y);
    pair.distance = cv::norm(a.center - b.center);
    pair.length_ratio = std::max(a.length, b.length) /
                        std::min(a.length, b.length);
    pair.pass = pair.angle_diff <= 20.0 && pair.y_diff <= 25.0 &&
                pair.distance >= 80.0 && pair.distance <= 220.0 &&
                pair.length_ratio <= 1.25;
    return pair;
}

}  // namespace

int main() {
    const std::vector<Bar> bars{
        {cv::Point2f(100, 100), 5.0, 160.0},
        {cv::Point2f(240, 102), 9.0, 158.0},
        {cv::Point2f(400, 100), 80.0, 160.0},
        {cv::Point2f(260, 300), 6.0, 160.0},
    };

    std::vector<Pair> pairs;
    for (std::size_t i = 0; i < bars.size(); ++i) {
        for (std::size_t j = i + 1; j < bars.size(); ++j) {
            pairs.push_back(measure(bars[i], bars[j], static_cast<int>(i),
                                    static_cast<int>(j)));
        }
    }
    for (const Pair& pair : pairs) {
        std::cout << pair.first << "-" << pair.second
                  << " angle=" << cvRound(pair.angle_diff)
                  << " y=" << cvRound(pair.y_diff)
                  << " distance=" << cvRound(pair.distance)
                  << " ratio=" << cvRound(pair.length_ratio * 100.0) / 100.0
                  << " pass=" << (pair.pass ? "yes" : "no") << '\n';
    }

    std::vector<bool> used(bars.size(), false);
    int accepted = 0;
    for (const Pair& pair : pairs) {
        if (pair.pass && !used[pair.first] && !used[pair.second]) {
            used[pair.first] = true;
            used[pair.second] = true;
            ++accepted;
        }
    }
    std::cout << "accepted=" << accepted << '\n';
    return 0;
}
