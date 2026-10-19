#include <cstdio>

int main() {
    unsigned char frame[11];

    frame[0] = 0xA5;
    frame[1] = 0x02;
    frame[2] = 0x00;
    frame[3] = 0x00;
    frame[4] = 0x8C;
    frame[5] = 0x01;
    frame[6] = 0x00;
    frame[7] = 0x12;
    frame[8] = 0x34;
    frame[9] = 0xD9;
    frame[10] = 0x34;

    for (int i = 0; i < 11; i++) {
        printf(i == 0 ? "%02X" : " %02X", frame[i]);
    }
    printf("\n");

    return 0;
}
