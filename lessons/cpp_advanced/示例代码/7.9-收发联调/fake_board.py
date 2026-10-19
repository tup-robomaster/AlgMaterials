#!/usr/bin/env python3
"""模拟板：在伪终端上扮演串口的另一端。"""

import os
import pty
import select
import subprocess
import sys
import time

REQUEST_LENGTH = 11
REPLY = bytes([0xA5, 0x02, 0x00, 0x01, 0xD2, 0x02, 0x00, 0x56, 0x78, 0x8B, 0xA8])


def crc8(data):
    crc = 0xFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 0x01:
                crc = ((crc >> 1) ^ 0x8C) & 0xFF
            else:
                crc = (crc >> 1) & 0xFF
    return crc


def crc16(data):
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 0x0001:
                crc = ((crc >> 1) ^ 0x8408) & 0xFFFF
            else:
                crc = (crc >> 1) & 0xFFFF
    return crc


def read_request(fd, timeout):
    data = b""
    deadline = time.monotonic() + timeout
    while len(data) < REQUEST_LENGTH:
        left = deadline - time.monotonic()
        if left <= 0:
            break
        ready, _, _ = select.select([fd], [], [], left)
        if not ready:
            break
        chunk = os.read(fd, REQUEST_LENGTH - len(data))
        if not chunk:
            break
        data += chunk
    return data


def request_ok(data):
    if len(data) != REQUEST_LENGTH or data[0] != 0xA5:
        return False
    if crc8(data[:4]) != data[4]:
        return False
    if crc16(data[:9]) != (data[9] | (data[10] << 8)):
        return False
    return True


def send_reply(fd):
    os.write(fd, REPLY[:3])
    time.sleep(0.05)
    os.write(fd, REPLY[3:])


def run_program(program):
    master, slave = pty.openpty()
    path = os.ttyname(slave)
    child = subprocess.Popen([program, path])
    try:
        data = read_request(master, 5.0)
        if not request_ok(data):
            print("模拟板：没有收到正确的请求帧", file=sys.stderr)
            child.kill()
            child.wait()
            return 1
        send_reply(master)
        try:
            code = child.wait(timeout=10)
        except subprocess.TimeoutExpired:
            child.kill()
            child.wait()
            print("模拟板：程序没有退出", file=sys.stderr)
            return 1
        if code != 0:
            print("模拟板：程序退出码不是 0", file=sys.stderr)
            return 1
        return 0
    finally:
        os.close(master)
        os.close(slave)


def manual():
    master, slave = pty.openpty()
    print("模拟板已启动，设备路径是：")
    print(os.ttyname(slave))
    print("在另一个终端里运行: ./demo 上面这个路径")
    print("等待请求……")
    data = read_request(master, 60.0)
    if request_ok(data):
        send_reply(master)
        print("已收到请求，应答已分两批发出。")
    else:
        print("没有收到正确的请求帧。")
    os.close(master)
    os.close(slave)
    return 0


def main():
    if len(sys.argv) == 1:
        return manual()
    if len(sys.argv) == 3 and sys.argv[1] == "--run":
        return run_program(sys.argv[2])
    print("用法: python3 fake_board.py [--run 程序]", file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
