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

    cfsetispeed(&t, B9600);
    cfsetospeed(&t, B9600);
    tcsetattr(fd, TCSANOW, &t);
    tcgetattr(fd, &t);
    printf("输入速度是 9600: %s\n", cfgetispeed(&t) == B9600 ? "是" : "否");
    printf("输出速度是 9600: %s\n", cfgetospeed(&t) == B9600 ? "是" : "否");

    cfsetispeed(&t, B115200);
    cfsetospeed(&t, B115200);
    tcsetattr(fd, TCSANOW, &t);
    tcgetattr(fd, &t);
    printf("输入速度是 115200: %s\n", cfgetispeed(&t) == B115200 ? "是" : "否");
    printf("输出速度是 115200: %s\n", cfgetospeed(&t) == B115200 ? "是" : "否");

    close(fd);
    close(master);
    return 0;
}
