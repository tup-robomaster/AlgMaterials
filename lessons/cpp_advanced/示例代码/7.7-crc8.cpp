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

int main() {
    const unsigned char check[9] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    printf("算例 %02X\n", crc8(check, 9));

    const unsigned char head[4] = {0xA5, 0x02, 0x00, 0x00};
    printf("帧 A 的 CRC8 %02X\n", crc8(head, 4));

    return 0;
}
