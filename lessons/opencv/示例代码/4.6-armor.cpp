#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

struct Bar {
    cv::Point2f center;
    cv::Size2f size;
    double angle = 0.0;
    cv::Rect box;
    std::vector<cv::Point> contour;
};

struct Pair {
    int first = 0;
    int second = 0;
    double angle_diff = 0.0;
    double y_diff = 0.0;
    double distance = 0.0;
    double length_ratio = 0.0;
    double center_mean = 0.0;
    bool geometry = false;
    bool accepted = false;
    cv::Point2f center;
    std::vector<cv::Point> points;
};

void place(cv::Mat& panel, const cv::Mat& image, int x, int y, int width,
           int height, const std::string& label) {
    const int label_height = 28;
    cv::rectangle(panel, cv::Rect(x, y, width, label_height),
                  cv::Scalar(245, 245, 245), cv::FILLED);
    cv::putText(panel, label, cv::Point(x + 8, y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);

    const int inner_width = width - 16;
    const int inner_height = height - label_height - 16;
    const double scale = std::min(static_cast<double>(inner_width) / image.cols,
                                  static_cast<double>(inner_height) / image.rows);
    const int shown_width = std::max(1, cvRound(image.cols * scale));
    const int shown_height = std::max(1, cvRound(image.rows * scale));
    cv::Mat shown;
    cv::resize(image, shown, cv::Size(shown_width, shown_height), 0, 0,
               cv::INTER_AREA);
    const int left = x + (width - shown_width) / 2;
    const int top = y + label_height + (inner_height - shown_height) / 2 + 8;
    shown.copyTo(panel(cv::Rect(left, top, shown_width, shown_height)));
}

cv::Mat redMask(const cv::Mat& hsv) {
    cv::Mat low;
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), low);
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(170, 80, 80), cv::Scalar(179, 255, 255),
                high);
    cv::Mat mask;
    cv::bitwise_or(low, high, mask);
    return mask;
}

cv::Mat blueMask(const cv::Mat& hsv) {
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(100, 80, 80), cv::Scalar(130, 255, 255),
                mask);
    return mask;
}

std::vector<Bar> findBars(const cv::Mat& mask) {
    cv::Mat opened;
    cv::morphologyEx(mask, opened, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(5, 25)));
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(opened.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<Bar> bars;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        const cv::Rect box = cv::boundingRect(contour);
        const double long_side = std::max(box.width, box.height);
        const double short_side = std::min(box.width, box.height);
        if (area < 500.0 || area > 8000.0 || long_side < 100.0 ||
            long_side > 230.0 ||
            short_side < 10.0 || short_side > 45.0 ||
            long_side / short_side < 2.0) {
            continue;
        }
        const cv::RotatedRect rect = cv::minAreaRect(contour);
        Bar bar;
        bar.center = rect.center;
        bar.size = rect.size;
        bar.angle = rect.angle;
        if (bar.size.width < bar.size.height) {
            bar.angle += 90.0;
        }
        while (bar.angle < 0.0) {
            bar.angle += 180.0;
        }
        while (bar.angle >= 180.0) {
            bar.angle -= 180.0;
        }
        bar.box = box;
        bar.contour = contour;
        bars.push_back(bar);
    }
    std::sort(bars.begin(), bars.end(), [](const Bar& a, const Bar& b) {
        return a.center.x < b.center.x;
    });
    return bars;
}

double angleDifference(double a, double b) {
    double diff = std::fabs(a - b);
    if (diff > 90.0) {
        diff = 180.0 - diff;
    }
    return diff;
}

double centerMean(const cv::Mat& gray, const Bar& a, const Bar& b) {
    cv::Rect box = a.box | b.box;
    const int dx = cvRound(box.width * 0.25);
    const int dy = cvRound(box.height * 0.25);
    box.x += dx;
    box.y += dy;
    box.width -= 2 * dx;
    box.height -= 2 * dy;
    box &= cv::Rect(0, 0, gray.cols, gray.rows);
    if (box.width <= 0 || box.height <= 0) {
        return 0.0;
    }
    return cv::mean(gray(box))[0];
}

std::vector<Pair> buildPairs(const std::vector<Bar>& bars,
                             const cv::Mat& gray) {
    std::vector<Pair> pairs;
    for (std::size_t i = 0; i < bars.size(); ++i) {
        for (std::size_t j = i + 1; j < bars.size(); ++j) {
            Pair pair;
            pair.first = static_cast<int>(i);
            pair.second = static_cast<int>(j);
            pair.angle_diff = angleDifference(bars[i].angle, bars[j].angle);
            pair.y_diff = std::fabs(bars[i].center.y - bars[j].center.y);
            pair.distance = cv::norm(bars[i].center - bars[j].center);
            const double long_i = std::max(bars[i].size.width,
                                           bars[i].size.height);
            const double long_j = std::max(bars[j].size.width,
                                           bars[j].size.height);
            pair.length_ratio = std::max(long_i, long_j) /
                                std::min(long_i, long_j);
            pair.geometry = pair.angle_diff <= 20.0 && pair.y_diff <= 25.0 &&
                            pair.distance >= 80.0 && pair.distance <= 220.0 &&
                            pair.length_ratio <= 1.25;
            pair.center = (bars[i].center + bars[j].center) * 0.5F;
            pair.center_mean = centerMean(gray, bars[i], bars[j]);
            std::vector<cv::Point> combined = bars[i].contour;
            combined.insert(combined.end(), bars[j].contour.begin(),
                            bars[j].contour.end());
            cv::RotatedRect box = cv::minAreaRect(combined);
            cv::Point2f points[4];
            box.points(points);
            for (const cv::Point2f& point : points) {
                pair.points.emplace_back(cvRound(point.x), cvRound(point.y));
            }
            pairs.push_back(pair);
        }
    }
    return pairs;
}

int acceptPairs(std::vector<Pair>& pairs, std::vector<bool>& used) {
    std::vector<int> order(pairs.size());
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        order[i] = static_cast<int>(i);
    }
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return pairs[a].center_mean > pairs[b].center_mean;
    });
    int accepted = 0;
    for (int index : order) {
        Pair& pair = pairs[index];
        if (!pair.geometry || pair.center_mean < 70.0 || used[pair.first] ||
            used[pair.second]) {
            continue;
        }
        pair.accepted = true;
        used[pair.first] = true;
        used[pair.second] = true;
        ++accepted;
    }
    return accepted;
}

void printPairs(const std::vector<Bar>& bars, const std::vector<Pair>& pairs,
                const std::string& color) {
    for (const Pair& pair : pairs) {
        if (!pair.geometry) {
            continue;
        }
        std::cout << color << " pair " << pair.first << "-" << pair.second
                  << " angle_diff=" << cvRound(pair.angle_diff)
                  << " y_diff=" << cvRound(pair.y_diff)
                  << " distance=" << cvRound(pair.distance)
                  << " ratio=" << cvRound(pair.length_ratio * 100.0) / 100.0
                  << " center_mean=" << cvRound(pair.center_mean)
                  << " accepted=" << (pair.accepted ? "yes" : "no") << '\n';
    }
    (void)bars;
}

}  // namespace

int main() {
    cv::Mat image = cv::imread("../素材/armor_frame.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    const std::vector<Bar> red_bars = findBars(redMask(hsv));
    const std::vector<Bar> blue_bars = findBars(blueMask(hsv));
    std::cout << "red bars=" << red_bars.size() << '\n';
    std::cout << "blue bars=" << blue_bars.size() << '\n';

    std::vector<Pair> red_pairs = buildPairs(red_bars, gray);
    std::vector<Pair> blue_pairs = buildPairs(blue_bars, gray);
    std::vector<bool> red_used(red_bars.size(), false);
    std::vector<bool> blue_used(blue_bars.size(), false);
    const int red_candidates = acceptPairs(red_pairs, red_used);
    const int blue_candidates = acceptPairs(blue_pairs, blue_used);

    printPairs(red_bars, red_pairs, "red");
    printPairs(blue_bars, blue_pairs, "blue");
    std::cout << "red candidates=" << red_candidates << '\n';
    std::cout << "blue candidates=" << blue_candidates << '\n';
    std::cout << "candidates=" << red_candidates + blue_candidates << '\n';

    cv::Mat red_bars_view = image.clone();
    for (const Bar& bar : red_bars) {
        cv::rectangle(red_bars_view, bar.box, cv::Scalar(0, 255, 0), 2);
    }
    cv::Mat blue_bars_view = image.clone();
    for (const Bar& bar : blue_bars) {
        cv::rectangle(blue_bars_view, bar.box, cv::Scalar(0, 255, 0), 2);
    }
    cv::Mat candidates_view = image.clone();
    for (const Pair& pair : red_pairs) {
        if (pair.accepted) {
            std::vector<std::vector<cv::Point>> polygon{pair.points};
            cv::polylines(candidates_view, polygon, true,
                          cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        }
    }
    for (const Pair& pair : blue_pairs) {
        if (pair.accepted) {
            std::vector<std::vector<cv::Point>> polygon{pair.points};
            cv::polylines(candidates_view, polygon, true,
                          cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        }
    }
    cv::putText(candidates_view,
                "candidates=" + std::to_string(red_candidates +
                                               blue_candidates),
                cv::Point(24, 52), cv::FONT_HERSHEY_SIMPLEX, 1.1,
                cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

    const int cell_width = 640;
    const int cell_height = 360;
    cv::Mat panel(2 * cell_height, 2 * cell_width, CV_8UC3,
                  cv::Scalar(255, 255, 255));
    place(panel, image, 0, 0, cell_width, cell_height, "original");
    place(panel, red_bars_view, cell_width, 0, cell_width, cell_height,
          "red bars=4");
    place(panel, blue_bars_view, 0, cell_height, cell_width, cell_height,
          "blue bars=2");
    place(panel, candidates_view, cell_width, cell_height, cell_width,
          cell_height, "candidates=2");

    const bool saved = cv::imwrite("out_armor.png", panel);
    std::cout << "saved out_armor.png: " << (saved ? "yes" : "no") << '\n';
    return 0;
}
