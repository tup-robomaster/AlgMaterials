#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace {

struct Component {
    int area = 0;
    cv::Rect box;
};

void describe(const std::string& title, const cv::Mat& mask) {
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    const int count = cv::connectedComponentsWithStats(
        mask, labels, stats, centroids, 8, CV_32S);
    std::vector<Component> components;
    for (int label = 1; label < count; ++label) {
        Component component;
        component.area = stats.at<int>(label, cv::CC_STAT_AREA);
        component.box = cv::Rect(stats.at<int>(label, cv::CC_STAT_LEFT),
                                 stats.at<int>(label, cv::CC_STAT_TOP),
                                 stats.at<int>(label, cv::CC_STAT_WIDTH),
                                 stats.at<int>(label, cv::CC_STAT_HEIGHT));
        components.push_back(component);
    }
    std::sort(components.begin(), components.end(),
              [](const Component& a, const Component& b) {
                  return a.box.x != b.box.x ? a.box.x < b.box.x
                                            : a.box.y < b.box.y;
              });
    std::cout << title << ": " << components.size() << '\n';
    for (const Component& component : components) {
        std::cout << "  area=" << component.area << "  box=("
                  << component.box.x << "," << component.box.y << ","
                  << component.box.width << "," << component.box.height
                  << ")\n";
    }
}

}  // namespace

int main() {
    cv::Mat mask(120, 240, CV_8UC1, cv::Scalar(0));
    cv::rectangle(mask, cv::Rect(20, 20, 50, 50), cv::Scalar(255),
                  cv::FILLED);
    cv::rectangle(mask, cv::Rect(90, 20, 50, 50), cv::Scalar(255),
                  cv::FILLED);
    cv::line(mask, cv::Point(69, 44), cv::Point(90, 44), cv::Scalar(255), 2);
    cv::circle(mask, cv::Point(210, 30), 2, cv::Scalar(255), cv::FILLED);
    cv::rectangle(mask, cv::Rect(20, 80, 30, 30), cv::Scalar(255),
                  cv::FILLED);
    cv::rectangle(mask, cv::Rect(55, 80, 30, 30), cv::Scalar(255),
                  cv::FILLED);

    describe("raw", mask);

    cv::Mat opened;
    cv::morphologyEx(mask, opened, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(5, 5)));
    describe("open 5x5", opened);

    cv::Mat closed;
    cv::morphologyEx(opened, closed, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT,
                                               cv::Size(9, 9)));
    describe("close 9x9 after open", closed);
    return 0;
}
