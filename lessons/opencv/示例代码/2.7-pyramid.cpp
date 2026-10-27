#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

void label(cv::Mat& panel, const std::string& text, int x, int y) {
    cv::rectangle(panel, cv::Rect(x, y, 260, 30), cv::Scalar(245, 245, 245),
                  cv::FILLED);
    cv::putText(panel, text, cv::Point(x + 8, y + 22),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(30, 30, 30), 1,
                cv::LINE_AA);
}

}  // namespace

int main() {
    const std::string scene_path = "../素材/synth_match_scene.png";
    const std::string template_path = "../素材/synth_match_template.png";
    cv::Mat scene = cv::imread(scene_path, cv::IMREAD_COLOR);
    cv::Mat templ = cv::imread(template_path, cv::IMREAD_COLOR);
    if (scene.empty() || templ.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    std::vector<cv::Mat> levels;
    levels.push_back(scene);
    while (levels.back().cols >= 2 * templ.cols &&
           levels.back().rows >= 2 * templ.rows &&
           levels.size() < 3) {
        cv::Mat next;
        cv::pyrDown(levels.back(), next);
        levels.push_back(next);
    }
    for (std::size_t i = 0; i < levels.size(); ++i) {
        std::cout << "pyramid " << i << ": " << levels[i].cols << 'x'
                  << levels[i].rows << '\n';
    }

    cv::Mat result;
    cv::matchTemplate(scene, templ, result, cv::TM_CCOEFF_NORMED);
    double best = 0.0;
    cv::Point best_location;
    cv::minMaxLoc(result, nullptr, &best, nullptr, &best_location);
    const cv::Point big_location = best_location;
    std::cout << "scale 1.00: score=" << cv::format("%.3f", best)
              << " loc=(" << best_location.x << ", " << best_location.y
              << ") size=" << templ.cols << 'x' << templ.rows << '\n';

    cv::Mat templ_half;
    cv::resize(templ, templ_half,
               cv::Size(templ.cols / 2, templ.rows / 2), 0, 0,
               cv::INTER_AREA);
    cv::matchTemplate(scene, templ_half, result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result, nullptr, &best, nullptr, &best_location);
    const cv::Point full_location(best_location.x, best_location.y);
    std::cout << "scale 0.50: score=" << cv::format("%.3f", best)
              << " loc=(" << best_location.x << ", " << best_location.y
              << ") size=" << templ_half.cols << 'x' << templ_half.rows
              << '\n';

    cv::Mat templ_quarter;
    cv::pyrDown(templ_half, templ_quarter);
    cv::matchTemplate(levels[1], templ_quarter, result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result, nullptr, &best, nullptr, &best_location);
    const cv::Point coarse_full(2 * best_location.x, 2 * best_location.y);
    std::cout << "coarse level 1: score=" << cv::format("%.3f", best)
              << " loc=(" << best_location.x << ", " << best_location.y
              << ") -> full=(" << coarse_full.x << ", " << coarse_full.y
              << ")\n";

    const int margin = 10;
    const int left = std::max(0, coarse_full.x - margin);
    const int top = std::max(0, coarse_full.y - margin);
    const int right = std::min(scene.cols,
                               coarse_full.x + templ_half.cols + margin);
    const int bottom = std::min(scene.rows,
                                coarse_full.y + templ_half.rows + margin);
    const cv::Rect search_roi(left, top, right - left, bottom - top);
    cv::Mat roi_result;
    cv::matchTemplate(scene(search_roi), templ_half, roi_result,
                      cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(roi_result, nullptr, &best, nullptr, &best_location);
    const cv::Point refined(left + best_location.x, top + best_location.y);
    std::cout << "refine level 0: score=" << cv::format("%.3f", best)
              << " loc=(" << refined.x << ", " << refined.y << ")\n";

    const int panel_width = 400;
    const int panel_height = 300;
    const int gap = 20;
    cv::Mat pyramid_panel(panel_height + 40, 3 * panel_width + 2 * gap, CV_8UC3,
                          cv::Scalar(255, 255, 255));
    for (int i = 0; i < 3; ++i) {
        cv::Mat shown;
        cv::resize(levels[i], shown,
                   cv::Size(levels[i].cols / 2, levels[i].rows / 2), 0, 0,
                   cv::INTER_AREA);
        const int x = i * (panel_width + gap);
        label(pyramid_panel,
              "level " + std::to_string(i) + ": " +
                  std::to_string(levels[i].cols) + "x" +
                  std::to_string(levels[i].rows),
              x, 0);
        shown.copyTo(pyramid_panel(
            cv::Rect(x, 40, shown.cols, shown.rows)));
    }

    cv::Mat match_panel = scene.clone();
    cv::rectangle(match_panel,
                  cv::Rect(big_location.x, big_location.y, templ.cols,
                           templ.rows),
                  cv::Scalar(0, 200, 0), 2);
    cv::rectangle(match_panel,
                  cv::Rect(full_location.x, full_location.y,
                           templ_half.cols, templ_half.rows),
                  cv::Scalar(255, 0, 0), 2);
    cv::putText(match_panel, "60x60", cv::Point(big_location.x, big_location.y - 8),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(0, 130, 0), 1,
                cv::LINE_AA);
    cv::putText(match_panel, "30x30", cv::Point(full_location.x, full_location.y - 8),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(255, 0, 0), 1,
                cv::LINE_AA);

    const bool pyramid_saved = cv::imwrite("out_pyramid.png", pyramid_panel);
    const bool match_saved = cv::imwrite("out_match.png", match_panel);
    std::cout << "saved out_pyramid.png: " << (pyramid_saved ? "yes" : "no")
              << '\n';
    std::cout << "saved out_match.png: " << (match_saved ? "yes" : "no")
              << '\n';
    return 0;
}
