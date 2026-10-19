#include <cstdio>

int main() {
    const unsigned char frame[11] = {
        0xA5, 0x02, 0x00, 0x00, 0x8C, 0x01, 0x00, 0x12, 0x34, 0xD9, 0x34
    };

    int data_length = frame[1] | (frame[2] << 8);
    int total = data_length + 9;
    int seq = frame[3];
    int cmd = frame[5] | (frame[6] << 8);

    printf("总长度 %d\n", total);
    printf("data_length %d\n", data_length);
    printf("seq %d\n", seq);
    printf("命令 0x%04X\n", cmd);
    printf("数据");
    for (int i = 0; i < data_length; i++) {
        printf(" %02X", frame[7 + i]);
    }
    printf("\n");
    printf("CRC8 %02X\n", frame[4]);
    printf("CRC16 %02X %02X\n", frame[total - 2], frame[total - 1]);

    return 0;
}
