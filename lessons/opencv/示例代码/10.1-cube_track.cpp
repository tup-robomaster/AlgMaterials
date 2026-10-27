#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

constexpr int kMinArea = 300;
constexpr double kGroupGap = 80.0;
constexpr double kGate = 80.0;
constexpr double kAlpha = 0.12;

struct Fragment {
    double area = 0.0;
    cv::Rect box;
    cv::Point2f center;
    std::vector<cv::Point> contour;
};

struct Target {
    double area = 0.0;
    cv::Rect box;
    cv::Point2f center;
    std::vector<int> parts;
};

double rectGap(const cv::Rect& a, const cv::Rect& b) {
    const int left = std::max(a.x, b.x);
    const int right = std::min(a.x + a.width, b.x + b.width);
    const int top = std::max(a.y, b.y);
    const int bottom = std::min(a.y + a.height, b.y + b.height);
    const double dx = std::max(0, left - right);
    const double dy = std::max(0, top - bottom);
    return std::hypot(dx, dy);
}

int findRoot(std::vector<int>& parent, int index) {
    while (parent[index] != index) {
        parent[index] = parent[parent[index]];
        index = parent[index];
    }
    return index;
}

void unite(std::vector<int>& parent, int a, int b) {
    const int ra = findRoot(parent, a);
    const int rb = findRoot(parent, b);
    if (ra != rb) {
        parent[rb] = ra;
    }
}

std::vector<Target> groupFragments(const std::vector<Fragment>& fragments) {
    const int count = static_cast<int>(fragments.size());
    std::vector<int> parent(count);
    std::iota(parent.begin(), parent.end(), 0);
    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (rectGap(fragments[i].box, fragments[j].box) <= kGroupGap) {
                unite(parent, i, j);
            }
        }
    }

    std::vector<Target> targets;
    std::vector<int> roots;
    for (int i = 0; i < count; ++i) {
        const int root = findRoot(parent, i);
        const auto found = std::find(roots.begin(), roots.end(), root);
        int group = 0;
        if (found == roots.end()) {
            group = static_cast<int>(roots.size());
            roots.push_back(root);
            targets.push_back(Target());
        } else {
            group = static_cast<int>(found - roots.begin());
        }
        Target& target = targets[group];
        target.area += fragments[i].area;
        target.parts.push_back(i);
        target.box = target.parts.size() == 1
                         ? fragments[i].box
                         : target.box | fragments[i].box;
    }

    for (Target& target : targets) {
        cv::Point2f sum(0.0F, 0.0F);
        for (int index : target.parts) {
            sum += fragments[index].center *
                   static_cast<float>(fragments[index].area);
        }
        target.center = sum / static_cast<float>(target.area);
    }
    return targets;
}

std::vector<Fragment> findFragments(const cv::Mat& hsv) {
    cv::Mat low;
    cv::inRange(hsv, cv::Scalar(150, 90, 90), cv::Scalar(179, 255, 255), low);
    cv::Mat high;
    cv::inRange(hsv, cv::Scalar(0, 90, 90), cv::Scalar(6, 255, 255), high);
    cv::Mat mask;
    cv::bitwise_or(low, high, mask);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(9, 9)));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<Fragment> fragments;
    for (const std::vector<cv::Point>& contour : contours) {
        const double area = cv::contourArea(contour);
        if (area < kMinArea) {
            continue;
        }
        const cv::Moments moments = cv::moments(contour);
        if (moments.m00 <= 0.0) {
            continue;
        }
        Fragment fragment;
        fragment.area = area;
        fragment.box = cv::boundingRect(contour);
        fragment.center = cv::Point2f(
            static_cast<float>(moments.m10 / moments.m00),
            static_cast<float>(moments.m01 / moments.m00));
        fragment.contour = contour;
        fragments.push_back(fragment);
    }
    return fragments;
}

void drawHud(cv::Mat& frame, int index, int total, int targets,
             int candidates) {
    cv::rectangle(frame, cv::Rect(0, 0, 370, 34), cv::Scalar(20, 20, 20),
                  cv::FILLED);
    const std::string text =
        "frame " + std::to_string(index) + "/" + std::to_string(total) +
        "  targets " + std::to_string(targets) + "  candidates " +
        std::to_string(candidates);
    cv::putText(frame, text, cv::Point(10, 24), cv::FONT_HERSHEY_SIMPLEX,
                0.55, cv::Scalar(240, 240, 240), 1, cv::LINE_AA);
}

cv::Mat makeTrajectoryImage(const cv::Mat& last,
                            const std::vector<cv::Point2f>& trail) {
    cv::Mat left = last.clone();
    for (std::size_t i = 1; i < trail.size(); ++i) {
        const double t = i / static_cast<double>(trail.size() - 1);
        const cv::Scalar color(255.0 * (1.0 - t), 80.0, 255.0 * t);
        cv::line(left, trail[i - 1], trail[i], color, 2, cv::LINE_AA);
    }
    if (!trail.empty()) {
        cv::circle(left, trail.front(), 7, cv::Scalar(240, 240, 240), 2,
                   cv::LINE_AA);
        cv::circle(left, trail.back(), 7, cv::Scalar(0, 0, 255), 2,
                   cv::LINE_AA);
        cv::putText(left, "start", trail.front() + cv::Point2f(10, -10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(240, 240, 240),
                    1, cv::LINE_AA);
        cv::putText(left, "end", trail.back() + cv::Point2f(10, 16),
                    cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(0, 0, 255), 1,
                    cv::LINE_AA);
    }

    cv::Mat right(480, 640, CV_8UC3, cv::Scalar(30, 30, 30));
    const cv::Point2f origin(320.0F, 240.0F);
    const double zoom = 4.0;
    const auto map = [&](const cv::Point2f& point) {
        return cv::Point(
            cvRound((point.x - origin.x) * zoom + right.cols / 2.0),
            cvRound((point.y - origin.y) * zoom + right.rows / 2.0));
    };
    for (int step = -4; step <= 4; ++step) {
        const int x = cvRound(right.cols / 2.0 + step * 20 * zoom);
        const int y = cvRound(right.rows / 2.0 + step * 20 * zoom);
        cv::line(right, cv::Point(x, 0), cv::Point(x, right.rows - 1),
                 cv::Scalar(55, 55, 55), 1);
        cv::line(right, cv::Point(0, y), cv::Point(right.cols - 1, y),
                 cv::Scalar(55, 55, 55), 1);
    }
    for (std::size_t i = 1; i < trail.size(); ++i) {
        const double t = i / static_cast<double>(trail.size() - 1);
        const cv::Scalar color(255.0 * (1.0 - t), 80.0, 255.0 * t);
        cv::line(right, map(trail[i - 1]), map(trail[i]), color, 2,
                 cv::LINE_AA);
    }
    if (!trail.empty()) {
        cv::circle(right, map(trail.front()), 7, cv::Scalar(240, 240, 240),
                   cv::FILLED, cv::LINE_AA);
        cv::circle(right, map(trail.back()), 7, cv::Scalar(0, 0, 255),
                   cv::FILLED, cv::LINE_AA);
    }
    cv::putText(right, "smoothed center (4x)", cv::Point(16, 34),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(230, 230, 230), 1,
                cv::LINE_AA);
    cv::putText(right, "grid 20 px   white = start   red = end",
                cv::Point(16, 460), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                cv::Scalar(190, 190, 190), 1, cv::LINE_AA);

    cv::Mat panel;
    cv::hconcat(left, right, panel);
    return panel;
}

}  // namespace

int main() {
    const std::string input = "../素材/cube.mp4";
    cv::VideoCapture capture(input);
    if (!capture.isOpened()) {
        std::cout << "无法打开 " << input << '\n';
        return 1;
    }

    const int width = cvRound(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = cvRound(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    const double fps = capture.get(cv::CAP_PROP_FPS);
    const int total = cvRound(capture.get(cv::CAP_PROP_FRAME_COUNT));

    const std::string video_path = "../素材/out_cube_track.mp4";
    cv::VideoWriter writer(video_path,
                           cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps,
                           cv::Size(width, height));
    if (!writer.isOpened()) {
        std::cout << "无法创建输出视频" << '\n';
        return 1;
    }

    bool tracking = false;
    cv::Point2f track(0.0F, 0.0F);
    std::vector<cv::Point2f> trail;
    std::vector<int> counts;
    std::vector<int> candidates;
    int detected = 0;
    int missed = 0;
    int reinit = 0;
    cv::Mat last;

    std::cout << "frames = " << total << '\n';
    std::cout << "frame  candidates  targets  cx     cy\n";

    int index = 0;
    while (true) {
        cv::Mat frame;
        capture >> frame;
        if (frame.empty()) {
            break;
        }

        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        const std::vector<Fragment> fragments = findFragments(hsv);
        const std::vector<Target> targets = groupFragments(fragments);
        counts.push_back(static_cast<int>(targets.size()));
        candidates.push_back(static_cast<int>(fragments.size()));

        cv::Mat shown = frame.clone();
        for (const Fragment& fragment : fragments) {
            cv::drawContours(shown, std::vector<std::vector<cv::Point>>{
                                          fragment.contour},
                            0, cv::Scalar(0, 220, 0), 1, cv::LINE_AA);
        }

        if (targets.empty()) {
            ++missed;
            tracking = false;
        } else {
            int chosen = 0;
            if (tracking) {
                double best = -1.0;
                for (int i = 0; i < static_cast<int>(targets.size()); ++i) {
                    const double distance = cv::norm(targets[i].center - track);
                    if (best < 0.0 || distance < best) {
                        best = distance;
                        chosen = i;
                    }
                }
                if (best > kGate) {
                    ++reinit;
                    track = targets[chosen].center;
                    tracking = true;
                } else {
                    track += (targets[chosen].center - track) *
                             static_cast<float>(kAlpha);
                    tracking = true;
                }
            } else {
                chosen = 0;
                for (int i = 1; i < static_cast<int>(targets.size()); ++i) {
                    if (targets[i].area > targets[chosen].area) {
                        chosen = i;
                    }
                }
                track = targets[chosen].center;
                tracking = true;
            }
            ++detected;
            trail.push_back(track);

            const Target& target = targets[chosen];
            cv::rectangle(shown, target.box, cv::Scalar(0, 200, 255), 2);
            cv::circle(shown, target.center, 4, cv::Scalar(0, 255, 255),
                       cv::FILLED, cv::LINE_AA);
            cv::circle(shown, track, 5, cv::Scalar(0, 0, 255), cv::FILLED,
                       cv::LINE_AA);
            for (std::size_t i = 1; i < trail.size(); ++i) {
                cv::line(shown, trail[i - 1], trail[i],
                         cv::Scalar(255, 220, 0), 2, cv::LINE_AA);
            }
        }

        drawHud(shown, index, total, static_cast<int>(targets.size()),
                static_cast<int>(fragments.size()));
        writer.write(shown);
        if (index % 15 == 0) {
            const cv::Point2f point =
                targets.empty() ? cv::Point2f(0.0F, 0.0F) : track;
            std::cout << std::setw(5) << index << "  " << std::setw(10)
                      << fragments.size() << "  " << std::setw(7)
                      << targets.size() << "  " << std::fixed
                      << std::setprecision(1) << std::setw(6) << point.x
                      << "  " << std::setw(6) << point.y << '\n';
        }
        last = frame.clone();
        ++index;
    }
    writer.release();

    if (last.empty() || trail.empty()) {
        std::cout << "没有读到有效帧" << '\n';
        return 1;
    }

    double path = 0.0;
    for (std::size_t i = 1; i < trail.size(); ++i) {
        path += cv::norm(trail[i] - trail[i - 1]);
    }
    const cv::Point2f mean =
        std::accumulate(trail.begin(), trail.end(), cv::Point2f(0.0F, 0.0F)) /
        static_cast<float>(trail.size());
    const auto x_bounds = std::minmax_element(
        trail.begin(), trail.end(),
        [](const cv::Point2f& a, const cv::Point2f& b) { return a.x < b.x; });
    const auto y_bounds = std::minmax_element(
        trail.begin(), trail.end(),
        [](const cv::Point2f& a, const cv::Point2f& b) { return a.y < b.y; });
    const auto count_bounds = std::minmax_element(counts.begin(), counts.end());
    const auto candidate_bounds =
        std::minmax_element(candidates.begin(), candidates.end());

    std::cout << "detected = " << detected << "  missed = " << missed << '\n';
    std::cout << "targets: min = " << *count_bounds.first
              << "  max = " << *count_bounds.second << '\n';
    std::cout << "candidates: min = " << *candidate_bounds.first
              << "  max = " << *candidate_bounds.second << '\n';
    std::cout << "reinit = " << reinit << '\n';
    std::cout << "trajectory points = " << trail.size() << '\n';
    std::cout << "path length = " << std::fixed << std::setprecision(1) << path
              << " px\n";
    std::cout << "track mean = (" << mean.x << ", " << mean.y << ")\n";
    std::cout << "track range = x " << x_bounds.first->x << ".."
              << x_bounds.second->x << "  y " << y_bounds.first->y << ".."
              << y_bounds.second->y << '\n';

    const cv::Mat trajectory = makeTrajectoryImage(last, trail);
    const std::string image_path = "../素材/out_cube_trajectory.png";
    const bool image_saved = cv::imwrite(image_path, trajectory);
    std::cout << "writer frames = " << index << '\n';

    cv::VideoCapture reader(video_path);
    if (!reader.isOpened()) {
        std::cout << "回读失败" << '\n';
        return 1;
    }
    std::cout << "readback frames = "
              << cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT)) << '\n';
    std::cout << "saved out_cube_track.mp4 = yes" << '\n';
    std::cout << "saved out_cube_trajectory.png = "
              << (image_saved ? "yes" : "no") << " " << trajectory.cols << "x"
              << trajectory.rows << '\n';
    return 0;
}
