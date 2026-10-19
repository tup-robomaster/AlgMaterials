#include <cstdint>
#include <cstdio>
#include <vector>

bool check_crc8(const std::vector<unsigned char>& buf) {
    unsigned char crc = 0xFF;
    for (int i = 0; i < 4; i++) {
        crc = (unsigned char)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x01) != 0) {
                crc = (unsigned char)((crc >> 1) ^ 0x8C);
            } else {
                crc = (unsigned char)(crc >> 1);
            }
        }
    }
    return crc == buf[4];
}

bool check_crc16(const std::vector<unsigned char>& buf, int total) {
    std::uint16_t crc = 0xFFFF;
    for (int i = 0; i < total - 2; i++) {
        crc = (std::uint16_t)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x0001) != 0) {
                crc = (std::uint16_t)((crc >> 1) ^ 0x8408);
            } else {
                crc = (std::uint16_t)(crc >> 1);
            }
        }
    }
    return (crc & 0xFF) == buf[total - 2] && (crc >> 8) == buf[total - 1];
}

bool take_frame(std::vector<unsigned char>& buf, int& cmd) {
    while (!buf.empty()) {
        if (buf[0] != 0xA5) {
            buf.erase(buf.begin());
            continue;
        }
        if ((int)buf.size() < 5) {
            return false;
        }
        if (!check_crc8(buf)) {
            buf.erase(buf.begin());
            continue;
        }

        int data_length = buf[1] | (buf[2] << 8);
        int total = data_length + 9;
        if ((int)buf.size() < total) {
            return false;
        }
        if (!check_crc16(buf, total)) {
            buf.erase(buf.begin());
            continue;
        }

        cmd = buf[5] | (buf[6] << 8);
        buf.erase(buf.begin(), buf.begin() + total);
        return true;
    }
    return false;
}

int main() {
    std::vector<unsigned char> buf;

    const unsigned char head[3] = {0xA5, 0x02, 0x00};
    buf.insert(buf.end(), head, head + 3);

    int cmd = 0;
    if (!take_frame(buf, cmd)) {
        printf("已有 %d 字节，还差 %d 字节\n",
               (int)buf.size(), 11 - (int)buf.size());
    }

    const unsigned char rest[8] = {
        0x00, 0x8C, 0x01, 0x00, 0x12, 0x34, 0xD9, 0x34
    };
    buf.insert(buf.end(), rest, rest + 8);

    if (take_frame(buf, cmd)) {
        printf("凑齐一帧，命令 0x%04X\n", cmd);
    }

    return 0;
}
