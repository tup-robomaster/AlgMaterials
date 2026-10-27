#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace {

struct Candidate {
    double area = 0.0;
    cv::Rect box;
    std::vector<cv::Point> poly;
    double ratio = 0.0;
    double max_cos = 0.0;
    bool accepted = false;
};

double sideLength(const cv::Point& a, const cv::Point& b) {
    return cv::norm(a - b);
}

}  // namespace

int main() {
    const std::string path = "../素材/synth_blocks.png";
    const cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取：" << path << '\n';
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat binary;
    cv::threshold(gray, binary, 0, 255,
                  cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    std::vector<Candidate> candidates;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        if (area < 1000.0) {
            continue;
        }
        Candidate candidate;
        candidate.area = area;
        candidate.box = cv::boundingRect(contour);
        const double perimeter = cv::arcLength(contour, true);
        cv::approxPolyDP(contour, candidate.poly, 0.02 * perimeter, true);
        if (candidate.poly.size() == 4 && cv::isContourConvex(candidate.poly)) {
            std::vector<double> sides;
            for (int i = 0; i < 4; ++i) {
                sides.push_back(
                    sideLength(candidate.poly[i], candidate.poly[(i + 1) % 4]));
            }
            const auto bounds =
                std::minmax_element(sides.begin(), sides.end());
            candidate.ratio = *bounds.second / *bounds.first;
            for (int i = 0; i < 4; ++i) {
                const cv::Point v1 = candidate.poly[(i + 3) % 4] -
                                     candidate.poly[i];
                const cv::Point v2 = candidate.poly[(i + 1) % 4] -
                                     candidate.poly[i];
                const double cosine =
                    std::abs(v1.dot(v2)) /
                    (cv::norm(v1) * cv::norm(v2));
                candidate.max_cos = std::max(candidate.max_cos, cosine);
            }
            candidate.accepted =
                candidate.ratio <= 1.20 && candidate.max_cos <= 0.15;
        }
        candidates.push_back(candidate);
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const Candidate& a, const Candidate& b) {
                  return a.box.x != b.box.x ? a.box.x < b.box.x
                                            : a.box.y < b.box.y;
              });

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "candidates = " << candidates.size() << '\n';
    int selected = 0;
    for (const Candidate& candidate : candidates) {
        const cv::Moments moments = cv::moments(candidate.poly);
        const double center_x = moments.m10 / moments.m00;
        const double center_y = moments.m01 / moments.m00;
        std::cout << "  area=" << std::setw(6) << candidate.area
                  << "  vertices=" << candidate.poly.size();
        if (candidate.poly.size() == 4) {
            std::cout << "  ratio=" << std::setprecision(2)
                      << candidate.ratio << "  max_cos="
                      << candidate.max_cos;
        } else {
            std::cout << "  ratio=--  max_cos=--";
        }
        std::cout << "  center=(" << std::setprecision(1) << center_x << ","
                  << center_y << ")  accept="
                  << (candidate.accepted ? "yes" : "no") << '\n';
        if (candidate.accepted) {
            ++selected;
        }
    }
    std::cout << "selected = " << selected << '\n';
    return 0;
}
