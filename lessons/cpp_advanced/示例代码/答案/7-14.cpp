#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

unsigned char crc8(const std::vector<unsigned char>& buf) {
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
    return crc;
}

std::uint16_t crc16(const std::vector<unsigned char>& buf) {
    std::uint16_t crc = 0xFFFF;
    for (int i = 0; i < (int)buf.size(); i++) {
        crc = (std::uint16_t)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x0001) != 0) {
                crc = (std::uint16_t)((crc >> 1) ^ 0x8408);
            } else {
                crc = (std::uint16_t)(crc >> 1);
            }
        }
    }
    return crc;
}

int main() {
    int seq = 0;
    int cmd = 0;
    if (!(std::cin >> seq >> cmd)) {
        return 1;
    }

    std::vector<unsigned char> data;
    int value = 0;
    std::cin >> std::hex;
    while (std::cin >> value) {
        data.push_back((unsigned char)value);
    }

    int len = (int)data.size();
    std::vector<unsigned char> frame;
    frame.push_back(0xA5);
    frame.push_back((unsigned char)(len & 0xFF));
    frame.push_back((unsigned char)((len >> 8) & 0xFF));
    frame.push_back((unsigned char)seq);
    frame.push_back(crc8(frame));
    frame.push_back((unsigned char)(cmd & 0xFF));
    frame.push_back((unsigned char)((cmd >> 8) & 0xFF));
    for (unsigned char b : data) {
        frame.push_back(b);
    }

    std::uint16_t check = crc16(frame);
    frame.push_back((unsigned char)(check & 0xFF));
    frame.push_back((unsigned char)((check >> 8) & 0xFF));

    for (int i = 0; i < (int)frame.size(); i++) {
        printf(i == 0 ? "%02X" : " %02X", frame[i]);
    }
    printf("\n");

    return 0;
}
