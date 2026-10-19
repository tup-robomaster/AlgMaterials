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
    printf("设置前 ICANON=%d ECHO=%d OPOST=%d\n",
           (t.c_lflag & ICANON) != 0,
           (t.c_lflag & ECHO) != 0,
           (t.c_oflag & OPOST) != 0);

    cfmakeraw(&t);
    tcsetattr(fd, TCSANOW, &t);

    tcgetattr(fd, &t);
    printf("设置后 ICANON=%d ECHO=%d OPOST=%d\n",
           (t.c_lflag & ICANON) != 0,
           (t.c_lflag & ECHO) != 0,
           (t.c_oflag & OPOST) != 0);
    printf("VMIN=%d VTIME=%d\n", t.c_cc[VMIN], t.c_cc[VTIME]);

    close(fd);
    close(master);
    return 0;
}
