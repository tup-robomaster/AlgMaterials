#!/usr/bin/env python3
"""简易裁判系统：接收 A5 帧，在本地网页上显示解码后的数据。

没有板子时，程序创建一对伪终端，把从端路径打印出来；发送端程序打开这个
路径，就像打开 /dev/ttyUSB0 一样。有板子时，用 --device 指定串口设备。

网页只在本机提供，地址是 http://127.0.0.1:端口。页面每 0.25 秒向
/api/state 取一次状态，把收到的帧按字段显示出来。

课程里的三条命令与数据格式是为演示编的，不对应官方业务命令：

    0x0001  机器人位置     id(1B) + x(2B 小端) + y(2B 小端)
    0x0002  机器人血量     id(1B) + hp(2B 小端)
    0x0003  剩余发弹量     id(1B) + ammo(2B 小端)
"""

import argparse
import json
import os
import pty
import select
import subprocess
import sys
import termios
import threading
import time
import tty
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

# 页面上的机器人顺序。编号是课程演示用的，和任何真实队伍无关。
ROBOTS = [
    (1, "英雄"),
    (2, "工程"),
    (3, "步兵 3"),
    (4, "步兵 4"),
    (7, "哨兵"),
]

COMMANDS = {
    0x0001: ("机器人位置", 5),
    0x0002: ("机器人血量", 3),
    0x0003: ("剩余发弹量", 3),
}

MAX_TOTAL = 100        # 一帧最长 100 字节，和课上的长度上限一致
LOG_LIMIT = 10         # 页面最多保留 10 条日志


def now_text():
    """返回 时:分:秒.毫秒，只用于显示。"""
    t = time.time()
    return time.strftime("%H:%M:%S", time.localtime(t)) + ".%03d" % int((t % 1) * 1000)


def hex_text(data):
    return " ".join("%02X" % b for b in data)


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


class State:
    """接收端保存的全部状态，HTTP 线程与接收线程共用。"""

    def __init__(self, device):
        self.lock = threading.Lock()
        self.device = device
        self.frames_ok = 0
        self.frames_bad = 0
        self.bytes = 0
        self.last_epoch = 0.0
        self.last_text = ""
        self.robots = {}
        for rid, name in ROBOTS:
            self.robots[rid] = {
                "id": rid,
                "name": name,
                "x": None,
                "y": None,
                "hp": None,
                "ammo": None,
                "time": "",
            }
        self.packets = {}
        for cmd in COMMANDS:
            self.packets[cmd] = None
        self.logs = []

    def add_log(self, status, frame, cmd=None, seq=None, length=None):
        entry = {
            "time": now_text(),
            "cmd": "" if cmd is None else "0x%04X" % cmd,
            "seq": "" if seq is None else str(seq),
            "length": "" if length is None else str(length),
            "status": status,
            "hex": hex_text(frame),
        }
        self.logs.insert(0, entry)
        del self.logs[LOG_LIMIT:]

    def good_frame(self, frame, data_length):
        cmd = frame[5] | (frame[6] << 8)
        seq = frame[3]
        data = frame[7:7 + data_length]
        stamp = now_text()

        with self.lock:
            self.frames_ok += 1
            self.last_epoch = time.time()
            self.last_text = stamp

            packet = {
                "time": stamp,
                "seq": seq,
                "length": data_length,
                "data": hex_text(data),
                "crc8": "%02X" % frame[4],
                "crc16": "%02X %02X" % (frame[-2], frame[-1]),
                "raw": hex_text(frame),
                "decoded": {},
                "note": "",
            }

            if cmd == 0x0001 and data_length == 5:
                rid = data[0]
                x = data[1] | (data[2] << 8)
                y = data[3] | (data[4] << 8)
                packet["decoded"] = {"机器人": rid, "X": x, "Y": y}
                if rid in self.robots:
                    robot = self.robots[rid]
                    robot["x"] = x
                    robot["y"] = y
                    robot["time"] = stamp
                else:
                    packet["note"] = "未知的机器人编号"
            elif cmd == 0x0002 and data_length == 3:
                rid = data[0]
                hp = data[1] | (data[2] << 8)
                packet["decoded"] = {"机器人": rid, "HP": hp}
                if rid in self.robots:
                    self.robots[rid]["hp"] = hp
                    self.robots[rid]["time"] = stamp
                else:
                    packet["note"] = "未知的机器人编号"
            elif cmd == 0x0003 and data_length == 3:
                rid = data[0]
                ammo = data[1] | (data[2] << 8)
                packet["decoded"] = {"机器人": rid, "弹量": ammo}
                if rid in self.robots:
                    self.robots[rid]["ammo"] = ammo
                    self.robots[rid]["time"] = stamp
                else:
                    packet["note"] = "未知的机器人编号"
            elif cmd in COMMANDS:
                packet["note"] = "数据长度与课程约定不符"

            if cmd in self.packets:
                self.packets[cmd] = packet

            self.add_log("CRC 通过", frame, cmd, seq, data_length)

    def bad_frame(self, status, frame):
        with self.lock:
            self.frames_bad += 1
            cmd = None
            seq = None
            length = None
            if len(frame) >= 3:
                length = frame[1] | (frame[2] << 8)
            if len(frame) >= 4:
                seq = frame[3]
            if len(frame) >= 7:
                cmd = frame[5] | (frame[6] << 8)
            self.add_log(status, frame, cmd, seq, length)

    def to_dict(self):
        with self.lock:
            age = None
            if self.last_epoch:
                age = time.time() - self.last_epoch
            packets = {}
            for cmd, packet in self.packets.items():
                packets["0x%04X" % cmd] = packet
            return {
                "device": self.device,
                "frames_ok": self.frames_ok,
                "frames_bad": self.frames_bad,
                "bytes": self.bytes,
                "last_time": self.last_text,
                "last_age": age,
                "robots": [self.robots[rid] for rid, _ in ROBOTS],
                "packets": packets,
                "logs": list(self.logs),
            }


class Parser:
    """按课上的收包顺序：找 A5、验 CRC8、读长度、等收齐、验 CRC16。"""

    def __init__(self, state):
        self.state = state
        self.buf = bytearray()

    def feed(self, chunk):
        with self.state.lock:
            self.state.bytes += len(chunk)
        self.buf.extend(chunk)
        while self.take_one():
            pass

    def take_one(self):
        while self.buf:
            if self.buf[0] != 0xA5:
                del self.buf[0]
                continue
            if len(self.buf) < 5:
                return False

            head = bytes(self.buf[:4])
            if crc8(head) != self.buf[4]:
                self.state.bad_frame("CRC8 错误", bytes(self.buf[:5]))
                del self.buf[0]
                continue

            data_length = self.buf[1] | (self.buf[2] << 8)
            total = data_length + 9
            if total > MAX_TOTAL:
                self.state.bad_frame("长度异常", bytes(self.buf[:5]))
                del self.buf[0]
                continue
            if len(self.buf) < total:
                return False

            frame = bytes(self.buf[:total])
            if crc16(frame[:total - 2]) != (frame[-2] | (frame[-1] << 8)):
                self.state.bad_frame("CRC16 错误", frame)
                del self.buf[0]
                continue

            del self.buf[:total]
            self.state.good_frame(frame, data_length)
            return True
        return False


class Handler(BaseHTTPRequestHandler):
    state = None
    dashboard = b""

    def log_message(self, fmt, *args):
        # 页面每 0.25 秒取一次状态，访问日志会把终端刷满，所以关掉。
        return

    def _send(self, status, body, content_type):
        try:
            self.send_response(status)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
        except (BrokenPipeError, ConnectionResetError):
            pass

    def do_GET(self):
        if self.path in ("/", "/index.html"):
            self._send(200, self.dashboard, "text/html; charset=utf-8")
        elif self.path == "/api/state":
            body = json.dumps(self.state.to_dict(), ensure_ascii=False)
            self._send(200, body.encode("utf-8"), "application/json; charset=utf-8")
        else:
            self._send(404, b"not found", "text/plain; charset=utf-8")


def is_own_terminal(path):
    """命令行所在的终端不能当串口：cfmakeraw 会让 Ctrl+C 失效。"""
    for fd in (0, 1, 2):
        try:
            tty_path = os.ttyname(fd)
        except OSError:
            continue
        if os.path.realpath(tty_path) == os.path.realpath(path):
            return True
    return False


def configure_serial(fd, baud):
    """把真实串口设成原始模式与指定波特率。"""
    speed = getattr(termios, "B%d" % baud, None)
    if speed is None:
        raise ValueError("不支持的波特率：%d" % baud)
    tty.setraw(fd)
    attrs = termios.tcgetattr(fd)
    attrs[4] = speed
    attrs[5] = speed
    termios.tcsetattr(fd, termios.TCSANOW, attrs)


def main():
    parser = argparse.ArgumentParser(description="简易裁判系统")
    parser.add_argument("--port", type=int, default=8000,
                        help="网页端口，默认 8000")
    parser.add_argument("--run", metavar="程序",
                        help="启动发送端程序，并把从端路径作为参数传给它")
    parser.add_argument("--device", metavar="设备",
                        help="使用已有的串口设备，不创建伪终端")
    parser.add_argument("--baud", type=int, default=115200,
                        help="真实串口的波特率，默认 115200")
    args = parser.parse_args()

    here = os.path.dirname(os.path.abspath(__file__))
    dashboard_path = os.path.join(here, "dashboard.html")
    if not os.path.exists(dashboard_path):
        print("找不到 dashboard.html", file=sys.stderr)
        return 2
    with open(dashboard_path, "rb") as f:
        Handler.dashboard = f.read()

    child = None
    slave = None
    if args.device:
        if is_own_terminal(args.device):
            print("这个路径是当前终端，不能当串口设备使用。", file=sys.stderr)
            print("请使用接收端打印的设备路径，或真实的串口设备。", file=sys.stderr)
            return 2
        try:
            fd = os.open(args.device, os.O_RDWR | os.O_NOCTTY)
            configure_serial(fd, args.baud)
        except (OSError, ValueError) as exc:
            print("打开串口失败：%s" % exc, file=sys.stderr)
            return 2
        device = args.device
    else:
        master, slave = pty.openpty()
        tty.setraw(slave)
        fd = master
        device = os.ttyname(slave)

    state = State(device)
    Handler.state = state

    try:
        server = ThreadingHTTPServer(("127.0.0.1", args.port), Handler)
    except OSError as exc:
        print("端口 %d 无法监听：%s" % (args.port, exc), file=sys.stderr)
        print("可以换一个端口：--port 8001", file=sys.stderr)
        os.close(fd)
        if slave is not None:
            os.close(slave)
        return 2

    threading.Thread(target=server.serve_forever, daemon=True).start()

    print("简易裁判系统已启动。")
    print("接收设备：%s" % device)
    print("页面地址：http://127.0.0.1:%d" % args.port)
    if args.run:
        child = subprocess.Popen([args.run, device])
    print("按 Ctrl+C 结束。")

    receive = Parser(state)
    child_done = False
    try:
        while True:
            ready, _, _ = select.select([fd], [], [], 0.2)
            if ready:
                chunk = os.read(fd, 4096)
                if not chunk:
                    break
                receive.feed(chunk)
            if child is not None and not child_done and child.poll() is not None:
                child_done = True
                print("发送端已退出，页面仍可查看已收到的数据；按 Ctrl+C 结束。")
    except KeyboardInterrupt:
        pass
    finally:
        if child is not None and child.poll() is None:
            child.terminate()
            child.wait()
        os.close(fd)
        if slave is not None:
            os.close(slave)
        server.shutdown()
        server.server_close()

    print("已结束。")
    return 0


if __name__ == "__main__":
    sys.exit(main())
