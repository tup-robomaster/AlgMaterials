#include <cstdio>

int main() {
    const unsigned char FLAG_READY = 0x01;
    const unsigned char FLAG_ARMED = 0x02;
    const unsigned char FLAG_ERROR = 0x04;

    unsigned char status = 0;
    status = status | FLAG_READY;
    status = status | FLAG_ERROR;
    printf("状态: %02X\n", status);

    if ((status & FLAG_READY) != 0) printf("已就绪\n");
    if ((status & FLAG_ARMED) != 0) printf("已武装\n");
    if ((status & FLAG_ERROR) != 0) printf("有错误\n");

    status = status & (unsigned char)~FLAG_ERROR;
    printf("清除错误后: %02X\n", status);

    return 0;
}
