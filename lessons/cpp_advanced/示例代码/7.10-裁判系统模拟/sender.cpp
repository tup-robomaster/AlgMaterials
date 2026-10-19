#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <random>
#include <termios.h>
#include <unistd.h>
#include <vector>

// 这是一份演示用的发送端，不算课上的例题，不逐行讲。
// 随机数用标准库的 std::mt19937（随机数引擎）和
// std::uniform_int_distribution（把结果限制在指定范围）。
// 本课不展开随机数，需要了解时可以搜索「C++ random mt19937」。

unsigned char crc8(const std::vector<unsigned char>& buf, int count) {
    unsigned char crc = 0xFF;
    for (int i = 0; i < count; i++) {
        crc = (unsigned char)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x01) != 0) {
                crc = (unsigned char)((crc >> 1) ^ 0x8C);
            } else {
                crc = (unsigned char)(crc >> 1);
            }
        }
    }
    return crc;
}

std::uint16_t crc16(const std::vector<unsigned char>& buf, int count) {
    std::uint16_t crc = 0xFFFF;
    for (int i = 0; i < count; i++) {
        crc = (std::uint16_t)(crc ^ buf[i]);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x0001) != 0) {
                crc = (std::uint16_t)((crc >> 1) ^ 0x8408);
            } else {
                crc = (std::uint16_t)(crc >> 1);
            }
        }
    }
    return crc;
}

void put_u16(std::vector<unsigned char>& data, int value) {
    data.push_back((unsigned char)(value & 0xFF));
    data.push_back((unsigned char)((value >> 8) & 0xFF));
}

std::vector<unsigned char> build_frame(int cmd, int seq,
                                       const std::vector<unsigned char>& data) {
    std::vector<unsigned char> frame;
    frame.push_back(0xA5);
    frame.push_back((unsigned char)(data.size() & 0xFF));
    frame.push_back((unsigned char)((data.size() >> 8) & 0xFF));
    frame.push_back((unsigned char)seq);
    frame.push_back(crc8(frame, 4));
    frame.push_back((unsigned char)(cmd & 0xFF));
    frame.push_back((unsigned char)((cmd >> 8) & 0xFF));
    for (unsigned char b : data) {
        frame.push_back(b);
    }
    std::uint16_t check = crc16(frame, (int)frame.size());
    frame.push_back((unsigned char)(check & 0xFF));
    frame.push_back((unsigned char)((check >> 8) & 0xFF));
    return frame;
}

bool write_all(int fd, const std::vector<unsigned char>& frame) {
    ssize_t written = write(fd, frame.data(), frame.size());
    return written == (ssize_t)frame.size();
}

bool is_own_terminal(const char* path) {
    const int fds[3] = {STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO};
    for (int i = 0; i < 3; i++) {
        const char* tty = ttyname(fds[i]);
        if (tty != nullptr && std::strcmp(tty, path) == 0) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    setvbuf(stdout, nullptr, _IOLBF, 0);

    if (argc < 2) {
        printf("用法: ./sender 设备路径\n");
        return 1;
    }

    if (is_own_terminal(argv[1])) {
        printf("这个路径是当前终端，不能当串口设备。\n");
        printf("请使用接收端打印的设备路径。\n");
        printf("若终端已经进入原始模式，执行 stty sane 恢复。\n");
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
    t.c_cc[VTIME] = 10;
    tcsetattr(fd, TCSANOW, &t);

    // 五台机器人轮流出现，保证每个状态都会被更新到。
    // 命令用长度为 7 的循环，和 5 台机器人的周期错开，
    // 免得某台机器人的某个字段总是正好落在坏帧上。
    const int robots[5] = {1, 2, 3, 4, 7};
    const int cmd_cycle[7] = {0x0001, 0x0002, 0x0003, 0x0002,
                              0x0001, 0x0003, 0x0002};

    std::random_device seed;
    std::mt19937 gen(seed());
    std::uniform_int_distribution<int> pos_dist(0, 2800);
    std::uniform_int_distribution<int> hp_dist(0, 500);
    std::uniform_int_distribution<int> ammo_dist(0, 300);

    int seq = 0;
    long tick = 0;
    printf("开始发送，每 0.2 秒一帧，按 Ctrl+C 停止。\n");

    while (true) {
        int robot = robots[tick % 5];
        int cmd = cmd_cycle[tick % 7];

        std::vector<unsigned char> data;
        data.push_back((unsigned char)robot);
        if (cmd == 0x0001) {
            put_u16(data, pos_dist(gen));
            put_u16(data, pos_dist(gen));
        } else if (cmd == 0x0002) {
            put_u16(data, hp_dist(gen));
        } else {
            put_u16(data, ammo_dist(gen));
        }

        // 每 3 帧故意发一个坏帧，三种错误轮流出现：
        // CRC8 错误、CRC16 错误、长度异常。
        bool bad = (tick % 3 == 2);
        long bad_kind = (tick / 3) % 3;

        if (bad && bad_kind == 2) {
            // 只发帧头，长度写成 150，整帧会超过接收端 100 字节的上限。
            std::vector<unsigned char> head;
            head.push_back(0xA5);
            head.push_back(150);
            head.push_back(0);
            head.push_back((unsigned char)seq);
            head.push_back(crc8(head, 4));
            if (!write_all(fd, head)) {
                printf("发送失败\n");
                close(fd);
                return 1;
            }
            printf("发送 0x%04X 机器人 %d（长度异常）\n", cmd, robot);
        } else {
            std::vector<unsigned char> frame = build_frame(cmd, seq, data);
            const char* note = "";
            if (bad && bad_kind == 0) {
                frame[4] = (unsigned char)(frame[4] ^ 0x01);
                note = "（CRC8 错误）";
            } else if (bad && bad_kind == 1) {
                frame[frame.size() - 1] =
                    (unsigned char)(frame[frame.size() - 1] ^ 0x01);
                note = "（CRC16 错误）";
            }
            if (!write_all(fd, frame)) {
                printf("发送失败\n");
                close(fd);
                return 1;
            }
            printf("发送 0x%04X 机器人 %d%s\n", cmd, robot, note);
        }

        seq++;
        tick++;
        usleep(200000);
    }

    close(fd);
    return 0;
}
