#include <cstdio>
#include <iostream>
#include <vector>

int main() {
    int seq = 0;
    int cmd = 0;
    int len = 0;
    if (!(std::cin >> seq >> cmd >> len)) {
        return 1;
    }

    std::vector<unsigned char> data;
    std::cin >> std::hex;
    for (int i = 0; i < len; i++) {
        int value = 0;
        std::cin >> value;
        data.push_back((unsigned char)value);
    }

    int crc8 = 0;
    int crc16 = 0;
    std::cin >> std::hex >> crc8 >> crc16;

    std::vector<unsigned char> frame;
    frame.push_back(0xA5);
    frame.push_back((unsigned char)(len & 0xFF));
    frame.push_back((unsigned char)((len >> 8) & 0xFF));
    frame.push_back((unsigned char)seq);
    frame.push_back((unsigned char)crc8);
    frame.push_back((unsigned char)(cmd & 0xFF));
    frame.push_back((unsigned char)((cmd >> 8) & 0xFF));
    for (unsigned char b : data) {
        frame.push_back(b);
    }
    frame.push_back((unsigned char)(crc16 & 0xFF));
    frame.push_back((unsigned char)((crc16 >> 8) & 0xFF));

    for (int i = 0; i < (int)frame.size(); i++) {
        printf(i == 0 ? "%02X" : " %02X", frame[i]);
    }
    printf("\n");

    return 0;
}
