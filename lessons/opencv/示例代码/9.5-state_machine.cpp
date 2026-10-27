#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

struct Bar {
    cv::Point2f center;
    cv::Point2f axis;
    cv::Point2f normal;
    float half_len = 0.0f;
    float half_wid = 0.0f;
    double area = 0.0;
    double angle = 0.0;
};

struct Pair {
    int left = -1;
    int right = -1;
    double score = 0.0;
    double angle_error = 0.0;
    double center_error = 0.0;
    double length_ratio = 1.0;
    cv::Point2f corners[4];
    cv::Mat plate;
    std::string color;
    int red_pixels = 0;
    int blue_pixels = 0;
};

float dot(const cv::Point2f& a, const cv::Point2f& b) {
    return a.x * b.x + a.y * b.y;
}

cv::Point2f normalize(const cv::Point2f& v) {
    const float n = std::sqrt(v.x * v.x + v.y * v.y);
    if (n < 1e-6f) {
        return cv::Point2f(0.0f, 1.0f);
    }
    return cv::Point2f(v.x / n, v.y / n);
}

Bar makeBar(const std::vector<cv::Point>& contour) {
    cv::Mat samples(static_cast<int>(contour.size()), 2, CV_64F);
    for (int i = 0; i < samples.rows; ++i) {
        samples.at<double>(i, 0) = contour[i].x;
        samples.at<double>(i, 1) = contour[i].y;
    }
    cv::PCA pca(samples, cv::Mat(), cv::PCA::DATA_AS_ROW);
    Bar bar;
    bar.center = cv::Point2f(
        static_cast<float>(pca.mean.at<double>(0, 0)),
        static_cast<float>(pca.mean.at<double>(0, 1)));
    bar.axis = normalize(cv::Point2f(
        static_cast<float>(pca.eigenvectors.at<double>(0, 0)),
        static_cast<float>(pca.eigenvectors.at<double>(0, 1))));
    if (bar.axis.y < 0.0f) {
        bar.axis.x = -bar.axis.x;
        bar.axis.y = -bar.axis.y;
    }
    bar.normal = cv::Point2f(bar.axis.y, -bar.axis.x);
    float min_a = 1e9f;
    float max_a = -1e9f;
    float min_n = 1e9f;
    float max_n = -1e9f;
    for (const auto& p : contour) {
        const cv::Point2f q = cv::Point2f(p) - bar.center;
        const float a = dot(q, bar.axis);
        const float n = dot(q, bar.normal);
        min_a = std::min(min_a, a);
        max_a = std::max(max_a, a);
        min_n = std::min(min_n, n);
        max_n = std::max(max_n, n);
    }
    bar.half_len = 0.5f * (max_a - min_a);
    bar.half_wid = 0.5f * (max_n - min_n);
    bar.area = cv::contourArea(contour);
    bar.angle = std::atan2(bar.axis.y, bar.axis.x) * 180.0 / CV_PI;
    return bar;
}

std::vector<Bar> findBars(const cv::Mat& gray) {
    cv::Mat mask;
    cv::threshold(gray, mask, 190, 255, cv::THRESH_BINARY);
    const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                                     cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    std::vector<Bar> bars;
    for (const auto& contour : contours) {
        const double area = cv::contourArea(contour);
        if (area < 500.0 || area > 10000.0) {
            continue;
        }
        const Bar bar = makeBar(contour);
        if (bar.half_wid < 1.0f) {
            continue;
        }
        const double aspect = bar.half_len / bar.half_wid;
        const double fill = area / (4.0 * bar.half_len * bar.half_wid);
        if (aspect < 2.5 || aspect > 12.0 || fill < 0.45 ||
            std::fabs(std::fabs(bar.angle) - 90.0) > 30.0) {
            continue;
        }
        bars.push_back(bar);
    }
    std::sort(bars.begin(), bars.end(), [](const Bar& a, const Bar& b) {
        return a.center.x < b.center.x;
    });
    return bars;
}

void classifyColor(Pair& pair) {
    cv::Mat hsv;
    cv::cvtColor(pair.plate, hsv, cv::COLOR_BGR2HSV);
    const cv::Rect roi(20, 15, 100, 50);
    for (int y = roi.y; y < roi.y + roi.height; ++y) {
        const cv::Vec3b* row = hsv.ptr<cv::Vec3b>(y);
        for (int x = roi.x; x < roi.x + roi.width; ++x) {
            const int h = row[x][0];
            const int s = row[x][1];
            const int v = row[x][2];
            if (s < 100 || v < 60) {
                continue;
            }
            if (h < 10 || h > 170) {
                ++pair.red_pixels;
            } else if (h > 100 && h < 130) {
                ++pair.blue_pixels;
            }
        }
    }
    pair.color = pair.red_pixels >= pair.blue_pixels ? "red" : "blue";
}

bool buildPair(const std::vector<Bar>& bars, int i, int j, Pair& pair) {
    const Bar& a = bars[i];
    const Bar& b = bars[j];
    const float dir = dot(a.axis, b.axis);
    if (dir < 0.0f) {
        return false;
    }
    const double angle_error = std::acos(std::min(1.0f, std::max(-1.0f, dir))) *
                               180.0 / CV_PI;
    if (angle_error > 8.0) {
        return false;
    }
    const cv::Point2f axis = normalize(a.axis + b.axis);
    const cv::Point2f normal(axis.y, -axis.x);
    const float dx = dot(b.center - a.center, normal);
    const float dy = dot(b.center - a.center, axis);
    const float mean_len = 0.5f * (a.half_len + b.half_len);
    const float full_len = 2.0f * mean_len;
    if (dx <= 0.0f || dx < 0.8f * full_len || dx > 2.5f * full_len) {
        return false;
    }
    if (std::fabs(dy) > 0.25f * full_len) {
        return false;
    }
    const double length_ratio = std::min(a.half_len, b.half_len) /
                                std::max(a.half_len, b.half_len);
    if (length_ratio < 0.8) {
        return false;
    }

    pair.left = i;
    pair.right = j;
    pair.angle_error = angle_error;
    pair.center_error = std::fabs(dy) / full_len;
    pair.length_ratio = length_ratio;
    pair.score = angle_error / 8.0 + pair.center_error / 0.25 +
                 (1.0 - length_ratio) / 0.2;

    pair.corners[0] = a.center - axis * a.half_len + normal * a.half_wid;
    pair.corners[1] = b.center - axis * b.half_len - normal * b.half_wid;
    pair.corners[2] = b.center + axis * b.half_len - normal * b.half_wid;
    pair.corners[3] = a.center + axis * a.half_len + normal * a.half_wid;
    return true;
}

cv::Mat warpPair(const cv::Mat& frame, const Pair& pair) {
    const cv::Point2f dst[4] = {
        cv::Point2f(0.0f, 0.0f), cv::Point2f(139.0f, 0.0f),
        cv::Point2f(139.0f, 79.0f), cv::Point2f(0.0f, 79.0f)};
    const cv::Mat transform = cv::getPerspectiveTransform(pair.corners, dst);
    cv::Mat plate;
    cv::warpPerspective(frame, plate, transform, cv::Size(140, 80),
                        cv::INTER_LINEAR);
    return plate;
}

}  // namespace

int main() {
    cv::VideoCapture cap("../素材/synth_armor.mp4");
    if (!cap.isOpened()) {
        std::cout << "无法打开 synth_armor.mp4\n";
        return 1;
    }
    const int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const double fps = cap.get(cv::CAP_PROP_FPS);

    cv::VideoWriter writer("../素材/out_armor_state.mp4",
                           cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps,
                           cv::Size(width, height));
    if (!writer.isOpened()) {
        std::cout << "无法创建 out_armor_state.mp4\n";
        return 1;
    }

    enum State { kCandidate, kConfirmed, kLost };
    State state = kCandidate;
    int hits = 0;
    int recapture_hits = 0;
    int misses = 0;
    int frame_index = 0;
    int detections = 0;
    int confirmed_frames = 0;
    int lost_events = 0;
    int recaptures = 0;
    std::string last_color = "-";

    cv::Mat frame;
    while (cap.read(frame)) {
        if (frame.empty()) {
            break;
        }
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        const std::vector<Bar> bars = findBars(gray);
        std::vector<Pair> pairs;
        for (std::size_t i = 0; i < bars.size(); ++i) {
            for (std::size_t j = i + 1; j < bars.size(); ++j) {
                Pair pair;
                if (buildPair(bars, static_cast<int>(i),
                              static_cast<int>(j), pair)) {
                    pair.plate = warpPair(frame, pair);
                    classifyColor(pair);
                    pairs.push_back(pair);
                }
            }
        }
        std::sort(pairs.begin(), pairs.end(),
                  [](const Pair& a, const Pair& b) {
                      return a.score < b.score;
                  });
        const bool detected = !pairs.empty();

        if (detected) {
            ++detections;
            const Pair& best = pairs.front();
            if (state == kLost) {
                ++recapture_hits;
                if (recapture_hits >= 3) {
                    state = kConfirmed;
                    hits = 3;
                    recapture_hits = 0;
                    ++recaptures;
                    std::cout << "frame=" << frame_index
                              << " state=lost->confirmed color=" << best.color
                              << '\n';
                }
            } else {
                ++hits;
                if (state == kCandidate && hits >= 3) {
                    state = kConfirmed;
                    std::cout << "frame=" << frame_index
                              << " state=candidate->confirmed color="
                              << best.color << '\n';
                }
            }
            if (best.color != last_color) {
                std::cout << "frame=" << frame_index << " color=" << last_color
                          << "->" << best.color << '\n';
                last_color = best.color;
            }
            misses = 0;
        } else {
            hits = 0;
            recapture_hits = 0;
            if (state == kConfirmed) {
                ++misses;
                if (misses >= 5) {
                    state = kLost;
                    recapture_hits = 0;
                    ++lost_events;
                    std::cout << "frame=" << frame_index
                              << " state=confirmed->lost\n";
                }
            }
        }

        if (state == kConfirmed) {
            ++confirmed_frames;
        }

        cv::Mat shown = frame.clone();
        if (detected) {
            const Pair& best = pairs.front();
            for (int k = 0; k < 4; ++k) {
                cv::line(shown,
                         cv::Point(cvRound(best.corners[k].x),
                                   cvRound(best.corners[k].y)),
                         cv::Point(cvRound(best.corners[(k + 1) % 4].x),
                                   cvRound(best.corners[(k + 1) % 4].y)),
                         cv::Scalar(40, 220, 40), 2, cv::LINE_AA);
            }
            cv::putText(shown, best.color, cv::Point(20, 32),
                        cv::FONT_HERSHEY_SIMPLEX, 0.75,
                        cv::Scalar(40, 220, 40), 2, cv::LINE_AA);
        }
        const char* state_text = "candidate";
        if (state == kConfirmed) {
            state_text = "confirmed";
        } else if (state == kLost) {
            state_text = "lost";
        }
        cv::putText(shown, state_text, cv::Point(20, 64),
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(40, 220, 255),
                    2, cv::LINE_AA);
        cv::putText(shown, cv::format("frame %d", frame_index),
                    cv::Point(20, 96), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                    cv::Scalar(230, 230, 230), 1, cv::LINE_AA);
        if (frame_index == 200) {
            cv::imwrite("../素材/out_armor_state.png", shown);
        }
        writer.write(shown);
        ++frame_index;
    }
    writer.release();

    cv::VideoCapture reader("../素材/out_armor_state.mp4");
    const int readback = reader.isOpened()
                             ? cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT))
                             : 0;
    std::cout << "frames = " << frame_index << '\n';
    std::cout << "detections = " << detections
              << " confirmed_frames = " << confirmed_frames << '\n';
    std::cout << "lost_events = " << lost_events
              << " recaptures = " << recaptures << '\n';
    std::cout << "saved out_armor_state.mp4 " << width << "x" << height
              << " frames=" << readback << ": "
              << (readback == frame_index ? "yes" : "no") << '\n';
    return 0;
}