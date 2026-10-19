#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

bool check_crc8(const std::vector<unsigned char>& buf) {
    unsigned char crc = 0xFF;
    for (int i = 0; i < 4; i++) {
        crc = (unsigned char)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x01) != 0) {
                crc = (unsigned char)((crc >> 1) ^ 0x8C);
            } else {
                crc = (unsigned char)(crc >> 1);
            }
        }
    }
    return crc == buf[4];
}

bool check_crc16(const std::vector<unsigned char>& buf, int total) {
    std::uint16_t crc = 0xFFFF;
    for (int i = 0; i < total - 2; i++) {
        crc = (std::uint16_t)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x0001) != 0) {
                crc = (std::uint16_t)((crc >> 1) ^ 0x8408);
            } else {
                crc = (std::uint16_t)(crc >> 1);
            }
        }
    }
    return (crc & 0xFF) == buf[total - 2] && (crc >> 8) == buf[total - 1];
}

bool take_frame(std::vector<unsigned char>& buf, int& cmd,
                std::vector<unsigned char>& data) {
    while (!buf.empty()) {
        if (buf[0] != 0xA5) {
            buf.erase(buf.begin());
            continue;
        }
        if ((int)buf.size() < 5) {
            return false;
        }
        if (!check_crc8(buf)) {
            buf.erase(buf.begin());
            continue;
        }

        int data_length = buf[1] | (buf[2] << 8);
        int total = data_length + 9;
        if (total > 100) {
            buf.erase(buf.begin());
            continue;
        }
        if ((int)buf.size() < total) {
            return false;
        }
        if (!check_crc16(buf, total)) {
            buf.erase(buf.begin());
            continue;
        }

        cmd = buf[5] | (buf[6] << 8);
        data.clear();
        for (int i = 0; i < data_length; i++) {
            data.push_back(buf[7 + i]);
        }
        buf.erase(buf.begin(), buf.begin() + total);
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("用法: ./demo 设备路径\n");
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd == -1) {
        printf("打开设备失败\n");
        return 1;
    }

    struct termios t;
    tcgetattr(fd, &t);
    cfmakeraw(&t);
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 50;
    tcsetattr(fd, TCSANOW, &t);

    const unsigned char request[11] = {
        0xA5, 0x02, 0x00, 0x00, 0x8C, 0x01, 0x00, 0x12, 0x34, 0xD9, 0x34
    };
    ssize_t written = write(fd, request, 11);
    if (written != 11) {
        printf("发送请求失败\n");
        close(fd);
        return 1;
    }

    std::vector<unsigned char> buf;
    int cmd = 0;
    int idle = 0;

    while (true) {
        unsigned char chunk[32];
        ssize_t n = read(fd, chunk, 32);

        if (n > 0) {
            idle = 0;
            for (ssize_t i = 0; i < n; i++) {
                buf.push_back(chunk[i]);
            }

            std::vector<unsigned char> data;
            while (take_frame(buf, cmd, data)) {
                if (cmd == 0x0002) {
                    printf("收到应答 0x%04X\n", cmd);
                    printf("数据");
                    for (unsigned char b : data) {
                        printf(" %02X", b);
                    }
                    printf("\n");
                    close(fd);
                    return 0;
                }
            }
        } else if (n == 0) {
            idle++;
            if (idle >= 1) {
                printf("等待应答超时\n");
                close(fd);
                return 1;
            }
        } else {
            printf("读取失败\n");
            close(fd);
            return 1;
        }
    }
}
