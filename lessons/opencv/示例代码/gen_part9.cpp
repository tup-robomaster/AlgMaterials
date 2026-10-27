#include <cmath>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace {

const int kWidth = 640;
const int kHeight = 480;
const int kFrames = 300;
const double kFps = 30.0;

void drawBackground(cv::Mat& frame, int index) {
    frame.setTo(cv::Scalar(42, 44, 46));
    for (int y = 40; y < kHeight; y += 60) {
        const int value = 54 + (y / 60) % 3 * 4;
        cv::line(frame, cv::Point(0, y), cv::Point(kWidth, y),
                 cv::Scalar(value, value, value), 1, cv::LINE_AA);
    }
    for (int x = 40; x < kWidth; x += 90) {
        const int value = 48 + (x / 90) % 2 * 5;
        cv::line(frame, cv::Point(x, 0), cv::Point(x, kHeight),
                 cv::Scalar(value, value, value), 1, cv::LINE_AA);
    }
    cv::Rect block(360, 330, 190, 95);
    cv::rectangle(frame, block, cv::Scalar(58, 60, 62), cv::FILLED);
    cv::rectangle(frame, block, cv::Scalar(78, 80, 82), 2, cv::LINE_AA);

    const int lamp_y = 80 + (index / 3) % 5;
    cv::rectangle(frame, cv::Rect(72, lamp_y, 8, 46),
                  cv::Scalar(230, 232, 234), cv::FILLED);
}

void drawDigit(cv::Mat& layer, int digit, const cv::Point& center) {
    const std::string text(1, static_cast<char>('0' + digit));
    int baseline = 0;
    const double scale = 1.15;
    const int thickness = 3;
    const cv::Size size = cv::getTextSize(
        text, cv::FONT_HERSHEY_SIMPLEX, scale, thickness, &baseline);
    const cv::Point origin(center.x - size.width / 2,
                           center.y + size.height / 2);
    cv::putText(layer, text, origin, cv::FONT_HERSHEY_SIMPLEX, scale,
                cv::Scalar(245, 245, 245, 255), thickness, cv::LINE_AA);
}

cv::Mat makeArmorLayer(int digit, bool blue) {
    cv::Mat layer(140, 220, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    cv::rectangle(layer, cv::Rect(20, 22, 180, 96),
                  cv::Scalar(30, 32, 34, 255), cv::FILLED);
    cv::rectangle(layer, cv::Rect(20, 22, 180, 96),
                  cv::Scalar(75, 77, 79, 255), 2, cv::LINE_AA);
    cv::rectangle(layer, cv::Rect(55, 40, 110, 60),
                  blue ? cv::Scalar(185, 62, 32, 255)
                       : cv::Scalar(32, 32, 185, 255),
                  cv::FILLED);
    cv::rectangle(layer, cv::Rect(55, 40, 110, 60),
                  cv::Scalar(215, 215, 215, 255), 1, cv::LINE_AA);
    cv::rectangle(layer, cv::Rect(25, 27, 18, 86),
                  cv::Scalar(246, 248, 250, 255), cv::FILLED);
    cv::rectangle(layer, cv::Rect(177, 27, 18, 86),
                  cv::Scalar(246, 248, 250, 255), cv::FILLED);
    drawDigit(layer, digit, cv::Point(110, 70));
    return layer;
}

void compose(cv::Mat& frame, const cv::Mat& layer, const cv::Point& center,
             double angle_deg) {
    const int canvas_w = 300;
    const int canvas_h = 220;
    cv::Mat canvas(canvas_h, canvas_w, CV_8UC4,
                   cv::Scalar(0, 0, 0, 0));
    layer.copyTo(canvas(cv::Rect((canvas_w - layer.cols) / 2,
                                 (canvas_h - layer.rows) / 2,
                                 layer.cols, layer.rows)));
    const cv::Mat rotation = cv::getRotationMatrix2D(
        cv::Point2f(canvas_w / 2.0f, canvas_h / 2.0f), angle_deg, 1.0);
    cv::Mat rotated;
    cv::warpAffine(canvas, rotated, rotation, canvas.size(), cv::INTER_LINEAR,
                   cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0, 0));

    const int left = center.x - canvas_w / 2;
    const int top = center.y - canvas_h / 2;
    for (int y = 0; y < rotated.rows; ++y) {
        const int fy = top + y;
        if (fy < 0 || fy >= frame.rows) {
            continue;
        }
        const cv::Vec4b* src = rotated.ptr<cv::Vec4b>(y);
        cv::Vec3b* dst = frame.ptr<cv::Vec3b>(fy);
        for (int x = 0; x < rotated.cols; ++x) {
            const int fx = left + x;
            if (fx < 0 || fx >= frame.cols) {
                continue;
            }
            const int alpha = src[x][3];
            if (alpha == 0) {
                continue;
            }
            for (int c = 0; c < 3; ++c) {
                dst[fx][c] = static_cast<uchar>(
                    (src[x][c] * alpha + dst[fx][c] * (255 - alpha)) / 255);
            }
        }
    }
}

void saveDigitSheet() {
    cv::Mat sheet(110, 5 * 140, CV_8UC3, cv::Scalar(248, 248, 246));
    for (int d = 1; d <= 5; ++d) {
        cv::Mat layer = makeArmorLayer(d, false);
        const cv::Rect roi(55, 40, 110, 60);
        cv::Mat plate;
        cv::cvtColor(layer(roi), plate, cv::COLOR_BGRA2BGR);
        const int x = (d - 1) * 140 + 15;
        plate.copyTo(sheet(cv::Rect(x, 20, 110, 60)));
        cv::putText(sheet, std::string(1, static_cast<char>('0' + d)),
                    cv::Point(x + 48, 100), cv::FONT_HERSHEY_SIMPLEX, 0.55,
                    cv::Scalar(30, 30, 30), 1, cv::LINE_AA);
    }
    const bool saved = cv::imwrite("../素材/synth_digits.png", sheet);
    std::cout << "saved synth_digits.png " << sheet.cols << "x" << sheet.rows
              << ": " << (saved ? "yes" : "no") << '\n';
}

}  // namespace

int main() {
    const std::string path = "../素材/synth_armor.mp4";
    cv::VideoWriter writer(path,
                           cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                           kFps, cv::Size(kWidth, kHeight));
    if (!writer.isOpened()) {
        std::cout << "无法创建 " << path << '\n';
        return 1;
    }

    for (int i = 0; i < kFrames; ++i) {
        cv::Mat frame(kHeight, kWidth, CV_8UC3);
        drawBackground(frame, i);

        const bool absent = i >= 130 && i < 170;
        if (!absent) {
            int digit = 3;
            bool blue = false;
            if (i >= 75 && i < 130) {
                digit = 2;
                blue = true;
            } else if (i >= 170 && i < 240) {
                digit = 5;
            } else if (i >= 240) {
                digit = 1;
                blue = true;
            }
            const double phase = 2.0 * CV_PI * i / kFrames;
            const int cx = cvRound(320.0 + 150.0 * std::sin(phase));
            const int cy = cvRound(240.0 + 38.0 * std::sin(2.0 * phase));
            const double angle = 9.0 * std::sin(3.0 * phase);
            const double scale = 0.95 + 0.05 * std::sin(2.0 * phase);
            cv::Mat layer = makeArmorLayer(digit, blue);
            cv::Mat scaled;
            cv::resize(layer, scaled, cv::Size(), scale, scale,
                       cv::INTER_LINEAR);
            compose(frame, scaled, cv::Point(cx, cy), angle);
        }
        writer.write(frame);
    }
    writer.release();

    cv::VideoCapture reader(path);
    std::cout << "saved " << path << ": "
              << (reader.isOpened() ? "yes" : "no") << '\n';
    if (reader.isOpened()) {
        std::cout << "width=" << cvRound(reader.get(cv::CAP_PROP_FRAME_WIDTH))
                  << " height="
                  << cvRound(reader.get(cv::CAP_PROP_FRAME_HEIGHT))
                  << " fps=" << reader.get(cv::CAP_PROP_FPS)
                  << " frames="
                  << cvRound(reader.get(cv::CAP_PROP_FRAME_COUNT)) << '\n';
    }
    saveDigitSheet();
    return 0;
}
