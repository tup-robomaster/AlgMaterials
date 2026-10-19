#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main() {
    int master = posix_openpt(O_RDWR | O_NOCTTY);
    if (master == -1) {
        printf("创建伪终端失败\n");
        return 1;
    }
    if (grantpt(master) != 0 || unlockpt(master) != 0) {
        printf("准备伪终端失败\n");
        close(master);
        return 1;
    }

    int fd = open(ptsname(master), O_RDWR | O_NOCTTY);
    if (fd == -1) {
        printf("打开从端失败\n");
        close(master);
        return 1;
    }

    struct termios t;
    tcgetattr(fd, &t);
    cfmakeraw(&t);
    tcsetattr(fd, TCSANOW, &t);

    unsigned char out[5] = {0xA5, 0x02, 0x00, 0x00, 0x8C};
    ssize_t written = write(master, out, 5);
    printf("写入 %ld 字节\n", (long)written);

    unsigned char in[5] = {0};
    ssize_t n = read(fd, in, 5);
    printf("读回 %ld 字节\n", (long)n);
    for (ssize_t i = 0; i < n; i++) {
        printf(i == 0 ? "%02X" : " %02X", in[i]);
    }
    printf("\n");

    close(fd);
    close(master);
    return 0;
}
