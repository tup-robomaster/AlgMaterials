#include <cstdint>
#include <cstdio>

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
    printf("算例 %04X\n", crc16(check, 9));

    const unsigned char body[9] = {
        0xA5, 0x02, 0x00, 0x00, 0x8C, 0x01, 0x00, 0x12, 0x34
    };
    std::uint16_t value = crc16(body, 9);
    printf("帧 A 的 CRC16 %04X\n", value);
    printf("低字节在前 %02X %02X\n", value & 0xFF, (value >> 8) & 0xFF);

    return 0;
}
