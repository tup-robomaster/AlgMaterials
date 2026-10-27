#include <iostream>
#include <vector>

#include <opencv2/core.hpp>

namespace {

void packPair(std::vector<uchar>& bytes, std::size_t offset, int first,
              int second) {
    bytes[offset] = static_cast<uchar>(first & 0xFF);
    bytes[offset + 1] = static_cast<uchar>(((first >> 8) & 0x0F) |
                                           ((second & 0x0F) << 4));
    bytes[offset + 2] = static_cast<uchar>((second >> 4) & 0xFF);
}

}  // namespace

int main() {
    const std::vector<int> values = {100, 1000, 2000, 4095};
    std::vector<uchar> packed(6, 0);
    packPair(packed, 0, values[0], values[1]);
    packPair(packed, 3, values[2], values[3]);

    std::cout << "packed bytes:";
    for (const uchar byte : packed) {
        std::cout << " " << static_cast<int>(byte);
    }
    std::cout << '\n';

    const int first = packed[0] | ((packed[1] & 0x0F) << 8);
    const int second = ((packed[1] >> 4) & 0x0F) | (packed[2] << 4);
    const int third = packed[3] | ((packed[4] & 0x0F) << 8);
    const int fourth = ((packed[4] >> 4) & 0x0F) | (packed[5] << 4);
    std::cout << "unpacked=" << first << " " << second << " " << third << " "
              << fourth << '\n';
    std::cout << "match="
              << (first == values[0] && second == values[1] &&
                          third == values[2] && fourth == values[3]
                      ? "yes"
                      : "no")
              << '\n';

    cv::Mat mono12(1, 4, CV_16UC1);
    for (int i = 0; i < 4; ++i) {
        mono12.at<ushort>(0, i) = static_cast<ushort>(values[i]);
    }
    std::cout << "mono12 type=" << mono12.type()
              << " channels=" << mono12.channels()
              << " elem=" << mono12.elemSize() << '\n';
    std::cout << "mono10 512->8bit: " << cvRound(512 * 255.0 / 1023.0) << '\n';
    std::cout << "mono12 2048->8bit: " << cvRound(2048 * 255.0 / 4095.0)
              << '\n';
    return 0;
}
