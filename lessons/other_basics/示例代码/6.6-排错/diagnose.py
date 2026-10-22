# 在本机依次制造五种常见错误，观察异常类型与判断方法。
# 只使用 127.0.0.1，不连接外部主机，不经过代理。
import errno
import json
import socket
import threading
from http.client import HTTPConnection
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HOST = "127.0.0.1"


def connection_refused():
    probe = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    probe.bind((HOST, 0))
    port = probe.getsockname()[1]
    try:
        socket.create_connection((HOST, port), timeout=1)
    except ConnectionRefusedError as error:
        print("1. 连接一个没有监听的端口")
        print(f"   异常：{type(error).__name__}")
        print("   判断：本机没有进程监听这个端口，服务可能没有启动。")
    finally:
        probe.close()


def address_in_use():
    first = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    first.bind((HOST, 0))
    address = first.getsockname()
    second = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        second.bind(address)
    except OSError as error:
        print("2. 同一个端口绑定两次")
        print(f"   异常：{type(error).__name__}")
        code = errno.errorcode.get(error.errno, "?")
        print(f"   errno：{error.errno}（{code}）")
        print("   判断：端口已被占用，换一个端口或先停止占用它的进程。")
    finally:
        second.close()
        first.close()


def read_timeout():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, 0))
    server.listen(1)
    client = socket.create_connection(server.getsockname(), timeout=1)
    client.settimeout(0.2)
    try:
        client.recv(1)
    except TimeoutError as error:
        print("3. 连接成功但对方不回应")
        print(f"   异常：{type(error).__name__}")
        print("   判断：连接已建立，但 0.2 秒内没有收到数据；"
              "检查对端是否发送、协议是否一致。")
    finally:
        client.close()
        server.close()


class NotFoundHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(404)
        self.send_header("Content-Length", "0")
        self.end_headers()

    def log_message(self, fmt, *args):
        pass


def http_not_found():
    server = ThreadingHTTPServer((HOST, 0), NotFoundHandler)
    server.daemon_threads = False
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    host, port = server.server_address
    connection = HTTPConnection(host, port, timeout=2)
    try:
        connection.request("GET", "/missing")
        response = connection.getresponse()
        response.read()
        print("4. 请求一个不存在的 HTTP 路径")
        print(f"   状态码：{response.status}")
        print("   判断：服务在运行，路径写错了；404 是正常响应，"
              "不是连接失败。")
    finally:
        connection.close()
        server.shutdown()
        server.server_close()
        thread.join(2)
    print(f"   服务线程已退出：{not thread.is_alive()}")


def bad_json():
    try:
        json.loads('{"a": 1,')
    except json.JSONDecodeError as error:
        print("5. 解析一段不完整的 JSON")
        print(f"   异常：{type(error).__name__}")
        print(f"   位置：第 {error.lineno} 行第 {error.colno} 列")
        print("   判断：数据不完整或被截断；先检查发送端与分隔方式。")


if __name__ == "__main__":
    connection_refused()
    address_in_use()
    read_timeout()
    http_not_found()
    bad_json()
