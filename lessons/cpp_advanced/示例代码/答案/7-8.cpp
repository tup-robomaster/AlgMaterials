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
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 10;
    tcsetattr(fd, TCSANOW, &t);

    tcgetattr(fd, &t);
    printf("VMIN=%d\n", t.c_cc[VMIN]);
    printf("VTIME=%d\n", t.c_cc[VTIME]);

    close(fd);
    close(master);
    return 0;
}
