#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat scene(8, 8, CV_8UC1);
    cv::RNG rng(20261024);
    rng.fill(scene, cv::RNG::UNIFORM, 0, 256);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            scene.at<uchar>(2 + row, 2 + col) =
                static_cast<uchar>(20 + 30 * row + 10 * col);
        }
    }

    cv::Mat level1;
    cv::Mat restored;
    cv::pyrDown(scene, level1);
    cv::pyrUp(level1, restored);
    std::cout << "level0=" << scene.cols << 'x' << scene.rows << '\n';
    std::cout << "level1=" << level1.cols << 'x' << level1.rows << '\n';
    std::cout << "pyrUp=" << restored.cols << 'x' << restored.rows << '\n';

    const cv::Mat templ = scene(cv::Rect(2, 2, 3, 3)).clone();
    cv::Mat result;
    cv::matchTemplate(scene, templ, result, cv::TM_CCOEFF_NORMED);
    double best = 0.0;
    cv::Point best_location;
    cv::minMaxLoc(result, nullptr, &best, nullptr, &best_location);
    std::cout << "match score=" << cv::format("%.3f", best)
              << " loc=(" << best_location.x << ", " << best_location.y
              << ")\n";
    return 0;
}
