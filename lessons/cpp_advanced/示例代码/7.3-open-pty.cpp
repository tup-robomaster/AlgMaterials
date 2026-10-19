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

    const char* slave_path = ptsname(master);
    if (slave_path == nullptr) {
        printf("取得从端路径失败\n");
        close(master);
        return 1;
    }

    int slave = open(slave_path, O_RDWR | O_NOCTTY);
    if (slave == -1) {
        printf("打开从端失败\n");
        close(master);
        return 1;
    }

    printf("伪终端创建成功，从端已打开\n");

    close(slave);
    close(master);
    return 0;
}
