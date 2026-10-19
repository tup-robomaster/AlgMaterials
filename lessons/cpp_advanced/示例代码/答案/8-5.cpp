#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

std::uint8_t crc8(const std::vector<unsigned char>& data, int count) {
    unsigned char crc = 0xFF;
    for (int i = 0; i < count; i++) {
        crc = static_cast<unsigned char>(crc ^ data[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x01) != 0) {
                crc = static_cast<unsigned char>((crc >> 1) ^ 0x8C);
            } else {
                crc = static_cast<unsigned char>(crc >> 1);
            }
        }
    }
    return crc;
}

std::uint16_t crc16(const std::vector<unsigned char>& data, int count) {
    std::uint16_t crc = 0xFFFF;
    for (int i = 0; i < count; i++) {
        crc = static_cast<std::uint16_t>(crc ^ data[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x0001) != 0) {
                crc = static_cast<std::uint16_t>((crc >> 1) ^ 0x8408);
            } else {
                crc = static_cast<std::uint16_t>(crc >> 1);
            }
        }
    }
    return crc;
}

struct Frame {
    unsigned char seq;
    std::uint16_t cmd;
    std::vector<unsigned char> data;
};

class Parser {
public:
    int good = 0;
    int bad = 0;

    void feed(const std::vector<unsigned char>& chunk) {
        buf_.insert(buf_.end(), chunk.begin(), chunk.end());
        while (take_one()) {
        }
    }

private:
    std::vector<unsigned char> buf_;

    bool take_one() {
        while (!buf_.empty()) {
            if (buf_[0] != 0xA5) {
                buf_.erase(buf_.begin());
                continue;
            }
            if (buf_.size() < 5) {
                return false;
            }
            if (crc8(buf_, 4) != buf_[4]) {
                bad++;
                buf_.erase(buf_.begin());
                continue;
            }

            int len = buf_[1] | (buf_[2] << 8);
            int total = len + 9;
            if (total > 100) {
                bad++;
                buf_.erase(buf_.begin());
                continue;
            }
            if (static_cast<int>(buf_.size()) < total) {
                return false;
            }

            std::uint16_t check =
                static_cast<std::uint16_t>(buf_[total - 2] |
                                           (buf_[total - 1] << 8));
            if (crc16(buf_, total - 2) != check) {
                bad++;
                buf_.erase(buf_.begin());
                continue;
            }

            Frame f;
            f.seq = buf_[3];
            f.cmd = static_cast<std::uint16_t>(buf_[5] | (buf_[6] << 8));
            f.data.assign(buf_.begin() + 7, buf_.begin() + 7 + len);
            buf_.erase(buf_.begin(), buf_.begin() + total);
            good++;
            print_frame(f);
            return true;
        }
        return false;
    }

    void print_frame(const Frame& f) const {
        printf("取出第 %d 帧：seq=%d 命令=0x%04X 数据=", good, f.seq,
               static_cast<unsigned int>(f.cmd));
        for (int i = 0; i < static_cast<int>(f.data.size()); i++) {
            if (i > 0) {
                printf(" ");
            }
            printf("%02X", f.data[i]);
        }
        printf("\n");
    }
};

int main() {
    std::vector<unsigned char> bytes;
    unsigned int value;
    std::cin >> std::hex;
    while (std::cin >> value) {
        bytes.push_back(static_cast<unsigned char>(value));
    }

    Parser parser;
    for (int i = 0; i < static_cast<int>(bytes.size()); i += 3) {
        int end = i + 3;
        if (end > static_cast<int>(bytes.size())) {
            end = static_cast<int>(bytes.size());
        }
        std::vector<unsigned char> chunk(bytes.begin() + i, bytes.begin() + end);
        parser.feed(chunk);
    }

    printf("好帧：%d\n", parser.good);
    printf("坏帧：%d\n", parser.bad);
    return 0;
}
