#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat mask = cv::Mat::zeros(9, 9, CV_8UC1);
    cv::rectangle(mask, cv::Rect(2, 2, 5, 5), cv::Scalar(255), cv::FILLED);

    cv::Mat distance;
    cv::distanceTransform(mask, distance, cv::DIST_L2, 5);
    double low = 0.0;
    double high = 0.0;
    cv::Point high_at;
    cv::minMaxLoc(distance, &low, &high, nullptr, &high_at);

    cv::Mat seeds;
    cv::threshold(distance, seeds, 2.0, 255.0, cv::THRESH_BINARY);
    seeds.convertTo(seeds, CV_8U);
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    const int count =
        cv::connectedComponentsWithStats(seeds, labels, stats, centroids, 8) - 1;

    cv::Mat background = mask.clone();
    const int flood_area =
        cv::floodFill(background, cv::Point(0, 0), cv::Scalar(128));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "distance max=" << high << " at=(" << high_at.x << ", "
              << high_at.y << ")\n";
    std::cout << "seeds=" << count << '\n';
    std::cout << "flood background=" << flood_area << '\n';
    return 0;
}
