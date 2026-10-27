#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat scene = cv::imread("../素材/synth_match_scene.png",
                               cv::IMREAD_COLOR);
    cv::Mat templ = cv::imread("../素材/synth_match_template.png",
                               cv::IMREAD_COLOR);
    if (scene.empty() || templ.empty()) {
        std::cout << "无法读取素材" << '\n';
        return 1;
    }

    cv::Mat level1;
    cv::Mat templ_half;
    cv::pyrDown(scene, level1);
    cv::pyrDown(templ, templ_half);
    std::cout << "pyramid 0: " << scene.cols << 'x' << scene.rows << '\n';
    std::cout << "pyramid 1: " << level1.cols << 'x' << level1.rows << '\n';

    cv::Mat result;
    double best = 0.0;
    cv::Point best_location;
    cv::matchTemplate(scene, templ, result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result, nullptr, &best, nullptr, &best_location);
    std::cout << "level 0 score=" << cv::format("%.3f", best)
              << " loc=(" << best_location.x << ", " << best_location.y
              << ")\n";

    cv::matchTemplate(level1, templ_half, result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result, nullptr, &best, nullptr, &best_location);
    std::cout << "level 1 score=" << cv::format("%.3f", best)
              << " loc=(" << best_location.x << ", " << best_location.y
              << ") -> full=(" << 2 * best_location.x << ", "
              << 2 * best_location.y << ")\n";
    return 0;
}
