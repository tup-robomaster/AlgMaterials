# 一个最小的 HTTP 服务：GET 与 POST、状态码、首部和 JSON。
# 客户端用 http.client 直连 127.0.0.1，不经过代理，也不读代理环境变量。
import json
import socket
import threading
from http.client import HTTPConnection
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HOST = "127.0.0.1"
CONFIG = {"camera": "A", "exposure_us": 5000, "enabled": True}
MAX_BODY = 64 * 1024
MAX_WORKERS = 4


class LimitedServer(ThreadingHTTPServer):
    """最多同时处理 MAX_WORKERS 个连接；超过时直接返回 503。"""
    allow_reuse_address = True
    daemon_threads = False

    def __init__(self, address, handler):
        super().__init__(address, handler)
        self.slots = threading.BoundedSemaphore(MAX_WORKERS)
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

    def send_json(self, status, obj):
        body = json.dumps(obj, ensure_ascii=False, sort_keys=True).encode("utf-8")
        try:
            self.send_response(status)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
        except (BrokenPipeError, ConnectionResetError, socket.timeout):
            pass

    def host_ok(self):
        host = self.headers.get("Host", "")
        if host not in self.server.allowed_hosts:
            self.send_json(400, {"error": "bad host"})
            return False
        return True

    def origin_ok(self):
        origin = self.headers.get("Origin")
        if origin is None:
            return True
        allowed = {
            f"http://{HOST}:{self.server.server_address[1]}",
            f"http://localhost:{self.server.server_address[1]}",
        }
        if origin not in allowed:
            self.send_json(403, {"error": "bad origin"})
            return False
        return True

    def do_GET(self):
        if not self.host_ok():
            return
        if self.path == "/config":
            self.send_json(200, CONFIG)
        else:
            self.send_json(404, {"error": "not found"})

    def do_POST(self):
        if not self.host_ok() or not self.origin_ok():
            return
        if self.path != "/echo":
            self.send_json(404, {"error": "not found"})
            return
        content_type = self.headers.get("Content-Type", "")
        if content_type.split(";")[0].strip().lower() != "application/json":
            self.send_json(415, {"error": "application/json required"})
            return
        raw_length = self.headers.get("Content-Length")
        if raw_length is None:
            self.send_json(411, {"error": "Content-Length required"})
            return
        try:
            length = int(raw_length)
        except ValueError:
            self.send_json(400, {"error": "bad Content-Length"})
            return
        if length < 0 or length > MAX_BODY:
            self.send_json(413, {"error": "body too large"})
            return
        try:
            data = json.loads(self.rfile.read(length).decode("utf-8"))
        except socket.timeout:
            self.send_json(408, {"error": "request timeout"})
            return
        except (UnicodeDecodeError, json.JSONDecodeError):
            self.send_json(400, {"error": "invalid JSON"})
            return
        if not isinstance(data, dict):
            self.send_json(400, {"error": "JSON object expected"})
            return
        a, b = data.get("a"), data.get("b")
        if not isinstance(a, (int, float)) or not isinstance(b, (int, float)):
            self.send_json(400, {"error": "a and b must be numbers"})
            return
        self.send_json(200, {"received": data, "sum": a + b})

    def log_message(self, fmt, *args):
        pass

    def log_error(self, fmt, *args):
        pass


server = LimitedServer((HOST, 0), Handler)
host, port = server.server_address
thread = threading.Thread(target=server.serve_forever, daemon=True)
thread.start()


def call(method, path, payload=None, content_type="application/json"):
    body = None
    headers = {}
    if payload is not None:
        body = json.dumps(payload).encode("utf-8")
        headers["Content-Type"] = content_type
    connection = HTTPConnection(host, port, timeout=2)
    try:
        connection.request(method, path, body=body, headers=headers)
        response = connection.getresponse()
        return response.status, response.getheader("Content-Type"), response.read()
    finally:
        connection.close()


try:
    status, content_type, body = call("GET", "/config")
    print("GET /config")
    print(f"状态：{status}")
    print(f"Content-Type：{content_type}")
    config = json.loads(body.decode("utf-8"))
    print(f"camera = {config['camera']}，exposure_us = "
          f"{config['exposure_us']}，enabled = {config['enabled']}")

    payload = {"a": 3, "b": 4}
    status, content_type, body = call("POST", "/echo", payload)
    print("POST /echo")
    print(f"状态：{status}")
    print(f"发送：{json.dumps(payload, sort_keys=True)}")
    echo = json.loads(body.decode("utf-8"))
    print(f"收到：{json.dumps(echo, ensure_ascii=False, sort_keys=True)}")

    status, content_type, body = call("GET", "/missing")
    print("GET /missing")
    print(f"状态：{status}")
    error = json.loads(body.decode("utf-8"))
    print(f"错误：{json.dumps(error, ensure_ascii=False, sort_keys=True)}")

    status, content_type, body = call("POST", "/echo", {"a": 1, "b": 2},
                                      content_type="text/plain")
    print("POST /echo（Content-Type 不是 JSON）")
    print(f"状态：{status}")
finally:
    server.shutdown()
    server.server_close()
    thread.join(2)
    print(f"服务线程已退出：{not thread.is_alive()}")
    print(f"剩余线程数：{len(threading.enumerate())}")
