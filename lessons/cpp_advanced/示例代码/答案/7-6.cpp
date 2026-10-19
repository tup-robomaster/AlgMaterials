#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
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
    printf("创建伪终端成功\n");

    int fd = open(ptsname(master), O_RDWR | O_NOCTTY);
    if (fd == -1) {
        printf("打开从端失败\n");
        close(master);
        return 1;
    }
    printf("打开从端成功\n");

    close(fd);
    close(master);
    printf("关闭成功\n");

    return 0;
}
