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


struct Params {
    double threshold = 190.0;
    double min_area = 500.0;
    double max_area = 10000.0;
    double min_aspect = 2.5;
    double max_aspect = 12.0;
    double min_fill = 0.45;
    double max_tilt = 30.0;
    double max_pair_angle = 8.0;
    double min_gap_ratio = 0.8;
    double max_gap_ratio = 2.5;
    double max_center_offset = 0.25;
    double min_length_ratio = 0.8;
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

bool buildPair(const std::vector<Bar>& bars, int i, int j,
               const Params& p, Pair& pair) {
    const Bar& a = bars[i];
    const Bar& b = bars[j];
    const float dir = dot(a.axis, b.axis);
    if (dir < 0.0f) {
        return false;
    }
    const double angle_error = std::acos(std::min(1.0f, std::max(-1.0f, dir))) *
                               180.0 / CV_PI;
    if (angle_error > p.max_pair_angle) {
        return false;
    }
    const cv::Point2f axis = normalize(a.axis + b.axis);
    const cv::Point2f normal(axis.y, -axis.x);
    const float dx = dot(b.center - a.center, normal);
    const float dy = dot(b.center - a.center, axis);
    const float mean_len = 0.5f * (a.half_len + b.half_len);
    const float full_len = 2.0f * mean_len;
    if (dx <= 0.0f || dx < p.min_gap_ratio * full_len ||
        dx > p.max_gap_ratio * full_len) {
        return false;
    }
    if (std::fabs(dy) > p.max_center_offset * full_len) {
        return false;
    }
    const double length_ratio = std::min(a.half_len, b.half_len) /
                                std::max(a.half_len, b.half_len);
    if (length_ratio < p.min_length_ratio) {
        return false;
    }

    pair.left = i;
    pair.right = j;
    pair.angle_error = angle_error;
    pair.center_error = std::fabs(dy) / full_len;
    pair.length_ratio = length_ratio;
    pair.score = angle_error / p.max_pair_angle +
                 pair.center_error / p.max_center_offset +
                 (1.0 - length_ratio) / (1.0 - p.min_length_ratio);

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

cv::Point2f pairCenter(const Pair& pair) {
    cv::Point2f center(0.0f, 0.0f);
    for (int k = 0; k < 4; ++k) {
        center += pair.corners[k];
    }
    return center * 0.25f;
}

int main() {
    cv::VideoCapture cap("../素材/synth_armor.mp4");
    if (!cap.isOpened()) {
        std::cout << "无法打开 synth_armor.mp4\n";
        return 1;
    }
    const int total = cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));
    const Params params;
    const cv::Rect vehicle_roi(60, 120, 520, 300);

    double gray_ms = 0.0;
    double mask_ms = 0.0;
    double contour_ms = 0.0;
    double pair_ms = 0.0;
    double total_ms = 0.0;
    int samples = 0;
    int detections = 0;
    cv::Mat sample_frame;
    cv::Point2f sample_local;
    cv::Point2f sample_full;
    int sample_index = -1;

    for (int index = 0; index < total; index += 3) {
        cap.set(cv::CAP_PROP_POS_FRAMES, index);
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            continue;
        }
        const cv::Mat roi = frame(vehicle_roi);
        const int64 start = cv::getTickCount();

        cv::Mat gray;
        cv::cvtColor(roi, gray, cv::COLOR_BGR2GRAY);
        const int64 after_gray = cv::getTickCount();

        cv::Mat mask;
        cv::threshold(gray, mask, params.threshold, 255, cv::THRESH_BINARY);
        const cv::Mat kernel = cv::getStructuringElement(
            cv::MORPH_RECT, cv::Size(3, 3));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
        const int64 after_mask = cv::getTickCount();

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL,
                         cv::CHAIN_APPROX_SIMPLE);
        std::vector<Bar> bars;
        for (const auto& contour : contours) {
            const double area = cv::contourArea(contour);
            if (area < params.min_area || area > params.max_area) {
                continue;
            }
            const Bar bar = makeBar(contour);
            if (bar.half_wid < 1.0f) {
                continue;
            }
            const double aspect = bar.half_len / bar.half_wid;
            const double fill =
                area / (4.0 * bar.half_len * bar.half_wid);
            if (aspect < params.min_aspect ||
                aspect > params.max_aspect || fill < params.min_fill ||
                std::fabs(std::fabs(bar.angle) - 90.0) > params.max_tilt) {
                continue;
            }
            bars.push_back(bar);
        }
        std::sort(bars.begin(), bars.end(), [](const Bar& a, const Bar& b) {
            return a.center.x < b.center.x;
        });
        const int64 after_contour = cv::getTickCount();

        std::vector<Pair> pairs;
        for (std::size_t i = 0; i < bars.size(); ++i) {
            for (std::size_t j = i + 1; j < bars.size(); ++j) {
                Pair pair;
                if (buildPair(bars, static_cast<int>(i), static_cast<int>(j),
                              params, pair)) {
                    pair.plate = warpPair(roi, pair);
                    classifyColor(pair);
                    pairs.push_back(pair);
                }
            }
        }
        std::sort(pairs.begin(), pairs.end(),
                  [](const Pair& a, const Pair& b) {
                      return a.score < b.score;
                  });
        const int64 after_pair = cv::getTickCount();
        if (!pairs.empty()) {
            ++detections;
            if (index == 39) {
                sample_index = index;
                sample_local = pairCenter(pairs.front());
                sample_full = sample_local + cv::Point2f(
                    static_cast<float>(vehicle_roi.x),
                    static_cast<float>(vehicle_roi.y));
                sample_frame = frame.clone();
                for (int k = 0; k < 4; ++k) {
                    const cv::Point2f a =
                        pairs.front().corners[k] +
                        cv::Point2f(static_cast<float>(vehicle_roi.x),
                                    static_cast<float>(vehicle_roi.y));
                    const cv::Point2f b =
                        pairs.front().corners[(k + 1) % 4] +
                        cv::Point2f(static_cast<float>(vehicle_roi.x),
                                    static_cast<float>(vehicle_roi.y));
                    cv::line(sample_frame, cv::Point(cvRound(a.x), cvRound(a.y)),
                             cv::Point(cvRound(b.x), cvRound(b.y)),
                             cv::Scalar(40, 220, 40), 2, cv::LINE_AA);
                }
                cv::rectangle(sample_frame, vehicle_roi,
                              cv::Scalar(40, 180, 255), 2, cv::LINE_AA);
            }
        }

        const double tick = cv::getTickFrequency();
        gray_ms += (after_gray - start) * 1000.0 / tick;
        mask_ms += (after_mask - after_gray) * 1000.0 / tick;
        contour_ms += (after_contour - after_mask) * 1000.0 / tick;
        pair_ms += (after_pair - after_contour) * 1000.0 / tick;
        total_ms += (after_pair - start) * 1000.0 / tick;
        ++samples;
    }

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "video_frames = " << total << " samples = " << samples
              << " detections = " << detections << '\n';
    std::cout << "params: threshold=" << params.threshold
              << " min_area=" << params.min_area
              << " max_area=" << params.max_area << '\n';
    std::cout << "params: aspect=[" << params.min_aspect << ','
              << params.max_aspect << "] fill=" << params.min_fill << '\n';
    std::cout << "roi = (" << vehicle_roi.x << ", " << vehicle_roi.y << ", "
              << vehicle_roi.width << ", " << vehicle_roi.height << ")\n";
    std::cout << "sample_frame = " << sample_index << '\n';
    std::cout << "local_center = (" << sample_local.x << ", "
              << sample_local.y << ")\n";
    std::cout << "full_center = (" << sample_full.x << ", "
              << sample_full.y << ")\n";
    std::cout << "time gray=" << gray_ms / samples << " ms\n";
    std::cout << "time mask=" << mask_ms / samples << " ms\n";
    std::cout << "time contours=" << contour_ms / samples << " ms\n";
    std::cout << "time pairing=" << pair_ms / samples << " ms\n";
    std::cout << "time total=" << total_ms / samples << " ms\n";
    if (!sample_frame.empty()) {
        cv::putText(sample_frame, "vehicle roi", cv::Point(70, 150),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(40, 180, 255),
                    2, cv::LINE_AA);
        const bool saved = cv::imwrite("../素材/out_armor_roi.png",
                                       sample_frame);
        std::cout << "saved out_armor_roi.png " << sample_frame.cols << "x"
                  << sample_frame.rows << ": " << (saved ? "yes" : "no")
                  << '\n';
    }
    return 0;
}