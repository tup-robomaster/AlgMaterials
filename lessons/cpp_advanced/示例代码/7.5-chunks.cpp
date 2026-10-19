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

    unsigned char first[3] = {0xA5, 0x02, 0x00};
    write(master, first, 3);

    unsigned char buf[8] = {0};
    ssize_t n = read(fd, buf, 8);
    printf("第一次读到 %ld 字节: ", (long)n);
    for (ssize_t i = 0; i < n; i++) {
        printf(i == 0 ? "%02X" : " %02X", buf[i]);
    }
    printf("\n");

    unsigned char second[5] = {0x00, 0x8C, 0x01, 0x00, 0x12};
    write(master, second, 5);

    n = read(fd, buf, 8);
    printf("第二次读到 %ld 字节: ", (long)n);
    for (ssize_t i = 0; i < n; i++) {
        printf(i == 0 ? "%02X" : " %02X", buf[i]);
    }
    printf("\n");

    close(fd);
    close(master);
    return 0;
}
