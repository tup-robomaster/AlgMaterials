#include <cstdint>
#include <cstdio>

unsigned char crc8(const unsigned char* data, int len) {
    unsigned char crc = 0xFF;
    for (int i = 0; i < len; i++) {
        crc = (unsigned char)(crc ^ data[i]);
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

std::uint16_t crc16(const unsigned char* data, int len) {
    std::uint16_t crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc = (std::uint16_t)(crc ^ data[i]);
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
    const unsigned char check[9] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    printf("CRC8 算例 %02X\n", crc8(check, 9));
    printf("CRC16 算例 %04X\n", crc16(check, 9));

    const unsigned char frame_a[11] = {
        0xA5, 0x02, 0x00, 0x00, 0x8C, 0x01, 0x00, 0x12, 0x34, 0xD9, 0x34
    };
    printf("帧 A 的 CRC8 %02X\n", crc8(frame_a, 4));
    printf("帧 A 的 CRC16 %04X\n", crc16(frame_a, 9));

    return 0;
}
