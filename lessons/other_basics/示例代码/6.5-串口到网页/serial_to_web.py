# 用伪终端模拟串口，把读到的 JSON 行接到本地网页。
# 不打开真实串口，不碰当前终端；只绑定 127.0.0.1。
import json
import os
import pty
import select
import sys
import threading
import time
import tty
from http.client import HTTPConnection
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HOST = "127.0.0.1"
MAX_WORKERS = 4
RECORDS = [
    {"seq": 1, "voltage": 12.1, "temperature": 25.1},
    {"seq": 2, "voltage": 12.3, "temperature": 25.4},
    {"seq": 3, "voltage": 12.2, "temperature": 25.6},
]

PAGE = """<!doctype html>
<html lang="zh-CN">
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>串口数据 · 本地网页</title>
<meta http-equiv="refresh" content="5">
<style>
  body {{ margin: 0; padding: 44px 24px; background: #f5f3ee; color: #24211d;
         font-family: system-ui, "Noto Sans CJK SC", sans-serif; }}
  main {{ max-width: 760px; margin: 0 auto; }}
  h1 {{ margin: 0 0 8px; font-size: 27px; }}
  .sub {{ color: #6b645c; margin: 0 0 24px; }}
  .band {{ display: flex; gap: 12px; margin-bottom: 20px; }}
  .chip {{ background: #fffdf9; border: 1px solid #d8d2c8; border-radius: 999px;
           padding: 7px 14px; font-size: 13px; color: #4d473f; }}
  table {{ width: 100%; border-collapse: collapse; background: #fffdf9;
           border: 1px solid #d8d2c8; border-radius: 8px; overflow: hidden; }}
  th, td {{ padding: 12px 16px; text-align: left; border-bottom: 1px solid #e7e1d7; }}
  th {{ background: #efeae1; font-size: 13px; color: #4d473f; }}
  tr:last-child td {{ border-bottom: 0; }}
  td {{ font-variant-numeric: tabular-nums; }}
  footer {{ margin-top: 20px; color: #6b645c; font-size: 13px; }}
</style>
<main>
  <h1>串口数据 · 本地网页</h1>
  <p class="sub">伪终端模拟串口，程序按行读取并解析 JSON；只绑定 127.0.0.1。</p>
  <div class="band">
    <span class="chip">已收到 {count} 条</span>
    <span class="chip">解析失败 {bad} 条</span>
    <span class="chip">页面每 5 秒刷新</span>
  </div>
  <table>
    <thead><tr><th>序号</th><th>电压 / V</th><th>温度 / °C</th></tr></thead>
    <tbody>
    {rows}
    </tbody>
  </table>
  <footer><a href="/latest">查看最新一条的 JSON</a>。模拟数据只在本机内存中。</footer>
</main>
"""


class LimitedServer(ThreadingHTTPServer):
    allow_reuse_address = True
    daemon_threads = False

    def __init__(self, address, handler):
        super().__init__(address, handler)
        self.slots = threading.BoundedSemaphore(MAX_WORKERS)
        self.lock = threading.Lock()
        self.records = []
        self.bad_lines = 0
        self.allowed_hosts = {
            f"{HOST}:{self.server_address[1]}",
            f"localhost:{self.server_address[1]}",
        }

    def process_request(self, request, client_address):
        if not self.slots.acquire(blocking=False):
            try:
                request.settimeout(2)
                request.sendall(b"HTTP/1.0 503 Service Unavailable\r\n"
                                b"Content-Length: 0\r\nConnection: close\r\n\r\n")
            finally:
                self.shutdown_request(request)
            return
        try:
            super().process_request(request, client_address)
        except Exception:
            self.slots.release()
            raise

    def process_request_thread(self, request, client_address):
        try:
            super().process_request_thread(request, client_address)
        finally:
            self.slots.release()


class Handler(BaseHTTPRequestHandler):
    def setup(self):
        super().setup()
        self.connection.settimeout(2)

    def host_ok(self):
        if self.headers.get("Host", "") not in self.server.allowed_hosts:
            self.send_text(400, "bad host")
            return False
        return True

    def send_text(self, status, text):
        body = text.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        self.end_headers()
        self.wfile.write(body)

    def send_html(self, status, text):
        body = text.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        self.send_header("X-Frame-Options", "DENY")
        self.send_header("Referrer-Policy", "no-referrer")
        self.send_header("Content-Security-Policy",
                         "default-src 'none'; style-src 'unsafe-inline'; "
                         "base-uri 'none'; form-action 'none'")
        self.end_headers()
        self.wfile.write(body)

    def send_json(self, status, obj):
        body = json.dumps(obj, ensure_ascii=False, sort_keys=True).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if not self.host_ok():
            return
        path = self.path.split("?", 1)[0]
        if path == "/favicon.ico":
            self.send_response(204)
            self.end_headers()
            return
        with self.server.lock:
            records = list(self.server.records)
            bad = self.server.bad_lines
        if path == "/latest":
            latest = records[-1] if records else None
            self.send_json(200, {"count": len(records), "bad_lines": bad,
                                 "latest": latest})
        elif path == "/":
            rows = []
            for record in records:
                rows.append(
                    "<tr><td>{}</td><td>{:.1f}</td><td>{:.1f}</td></tr>".format(
                        int(record["seq"]), float(record["voltage"]),
                        float(record["temperature"])))
            if not rows:
                rows = ['<tr><td colspan="3">等待数据</td></tr>']
            page = PAGE.format(count=len(records), bad=bad,
                               rows="\n    ".join(rows))
            self.send_html(200, page)
        else:
            self.send_text(404, "not found")

    def log_message(self, fmt, *args):
        pass

    def log_error(self, fmt, *args):
        pass


def start_bridge():
    master_fd, slave_fd = pty.openpty()
    path = os.ttyname(slave_fd)
    tty.setraw(slave_fd)
    read_fd = os.open(path, os.O_RDWR | os.O_NOCTTY)
    tty.setraw(read_fd)
    os.close(slave_fd)
    return master_fd, read_fd, path


def simulate(master_fd):
    for record in RECORDS:
        line = json.dumps(record, ensure_ascii=False) + "\n"
        os.write(master_fd, line.encode("utf-8"))
        time.sleep(0.05)


def read_lines(server, read_fd, ready, stop):
    buffer = b""
    while not stop.is_set():
        try:
            readable, _, _ = select.select([read_fd], [], [], 0.2)
        except (OSError, ValueError):
            break
        if not readable:
            continue
        try:
            chunk = os.read(read_fd, 1024)
        except OSError:
            break
        if not chunk:
            break
        buffer += chunk
        while b"\n" in buffer:
            line, buffer = buffer.split(b"\n", 1)
            if not line.strip():
                continue
            try:
                record = json.loads(line.decode("utf-8"))
                if not isinstance(record, dict):
                    raise ValueError
                for key in ("seq", "voltage", "temperature"):
                    if not isinstance(record.get(key), (int, float)):
                        raise ValueError
            except (UnicodeDecodeError, json.JSONDecodeError, ValueError):
                with server.lock:
                    server.bad_lines += 1
                continue
            with server.lock:
                server.records.append(record)
                if len(server.records) >= len(RECORDS):
                    ready.set()


def get_json(host, port, path):
    connection = HTTPConnection(host, port, timeout=2)
    try:
        connection.request("GET", path)
        response = connection.getresponse()
        return (response.status, response.getheader("Content-Type"),
                response.read())
    finally:
        connection.close()


def start(port):
    server = LimitedServer((HOST, port), Handler)
    master_fd, read_fd, path = start_bridge()
    ready = threading.Event()
    stop = threading.Event()
    writer = threading.Thread(target=simulate, args=(master_fd,), daemon=True)
    reader = threading.Thread(target=read_lines,
                              args=(server, read_fd, ready, stop), daemon=True)
    writer.start()
    reader.start()
    return server, master_fd, read_fd, path, ready, stop, writer, reader


def stop_all(server, master_fd, read_fd, stop, reader, writer, server_thread=None):
    stop.set()
    reader.join(2)
    writer.join(2)
    for fd in (read_fd, master_fd):
        try:
            os.close(fd)
        except OSError:
            pass
    if server_thread is not None:
        server.shutdown()
    server.server_close()
    if server_thread is not None:
        server_thread.join(2)


def run_self_test():
    server, master_fd, read_fd, path, ready, stop, writer, reader = start(0)
    host, port = server.server_address
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    try:
        print(f"模拟串口路径：{path}")
        if not ready.wait(2):
            raise SystemExit("2 秒内没有收到 3 条记录")
        status, content_type, body = get_json(host, port, "/latest")
        print(f"GET /latest 状态：{status}，类型：{content_type}")
        print(json.dumps(json.loads(body.decode("utf-8")),
                         ensure_ascii=False, sort_keys=True))
        status, content_type, body = get_json(host, port, "/")
        print(f"GET / 状态：{status}，类型：{content_type}")
        title = body.decode("utf-8").split("<title>", 1)[1].split("</title>", 1)[0]
        print(f"页面标题：{title}")
    finally:
        stop_all(server, master_fd, read_fd, stop, reader, writer, thread)
    print(f"串口读取线程已退出：{not reader.is_alive()}")
    print(f"服务线程已退出：{not thread.is_alive()}")


def serve(port):
    try:
        server, master_fd, read_fd, path, ready, stop, writer, reader = start(port)
    except OSError as error:
        print(f"无法启动服务：{error.strerror}")
        return
    host, actual = server.server_address
    print(f"模拟串口路径：{path}")
    print(f"服务地址：http://{host}:{actual}/")
    print("只绑定回环地址；模拟数据写完后页面仍可刷新；按 Ctrl+C 停止。")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n收到 Ctrl+C，正在停止。")
    finally:
        stop_all(server, master_fd, read_fd, stop, reader, writer)
        print("服务已停止。")


if __name__ == "__main__":
    arguments = sys.argv[1:]
    if arguments and arguments[0] == "--serve":
        requested = 0
        if len(arguments) > 1:
            try:
                requested = int(arguments[1])
            except ValueError:
                raise SystemExit("端口必须是整数。")
            if not 1 <= requested <= 65535:
                raise SystemExit("端口必须在 1–65535 之间。")
        serve(requested)
    else:
        run_self_test()
