# 一个只绑定回环地址的小服务：网页、JSON 状态、并发上限与超时。
# 默认自检一次后退出；加 --serve 才常驻，供浏览器打开。
import html
import json
import sys
import threading
from http.client import HTTPConnection
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HOST = "127.0.0.1"
MAX_WORKERS = 4

PAGE = """<!doctype html>
<html lang="zh-CN">
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>本地服务 · 运行状态</title>
<meta http-equiv="refresh" content="10">
<style>
  body {{ margin: 0; padding: 48px 24px; background: #f5f3ee; color: #24211d;
         font-family: system-ui, "Noto Sans CJK SC", sans-serif; }}
  main {{ max-width: 720px; margin: 0 auto; }}
  h1 {{ margin: 0 0 8px; font-size: 28px; }}
  .sub {{ color: #6b645c; margin: 0 0 28px; }}
  .cards {{ display: grid; grid-template-columns: repeat(3, 1fr); gap: 14px; }}
  .card {{ background: #fffdf9; border: 1px solid #d8d2c8; border-radius: 8px;
           padding: 18px 20px; }}
  .k {{ color: #6b645c; font-size: 13px; margin-bottom: 8px; }}
  .v {{ font-size: 26px; font-variant-numeric: tabular-nums; }}
  .ok {{ color: #2f6b46; }}
  footer {{ margin-top: 28px; color: #6b645c; font-size: 13px; }}
  a {{ color: #6a5490; }}
</style>
<main>
  <h1>本地服务 · 运行状态</h1>
  <p class="sub">只绑定 127.0.0.1，其他机器无法访问。</p>
  <div class="cards">
    <div class="card"><div class="k">请求次数</div><div class="v">{hits}</div></div>
    <div class="card"><div class="k">最近路径</div><div class="v">{recent}</div></div>
    <div class="card"><div class="k">状态</div><div class="v ok">运行中</div></div>
  </div>
  <footer>
    每 10 秒自动刷新；<a href="/status">查看 JSON 状态</a>。
    这个页面由本机程序生成，不连接外部网络。
  </footer>
</main>
"""


class LimitedServer(ThreadingHTTPServer):
    allow_reuse_address = True
    daemon_threads = False

    def __init__(self, address, handler):
        super().__init__(address, handler)
        self.slots = threading.BoundedSemaphore(MAX_WORKERS)
        self.lock = threading.Lock()
        self.hits = 0
        self.recent = "/"
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
        if path not in ("/", "/status"):
            self.send_text(404, "not found")
            return
        with self.server.lock:
            self.server.hits += 1
            self.server.recent = path
            hits = self.server.hits
            recent = self.server.recent
        if path == "/":
            page = PAGE.format(hits=hits, recent=html.escape(recent))
            self.send_html(200, page)
        else:
            self.send_json(200, {"ok": True, "hits": hits,
                                 "message": "服务运行正常"})

    def log_message(self, fmt, *args):
        pass

    def log_error(self, fmt, *args):
        pass


def run_self_test(server):
    host, port = server.server_address
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()

    def get(path):
        connection = HTTPConnection(host, port, timeout=2)
        try:
            connection.request("GET", path)
            response = connection.getresponse()
            return (response.status, response.getheader("Content-Type"),
                    response.read())
        finally:
            connection.close()

    try:
        status, content_type, body = get("/")
        print(f"GET / 状态：{status}，类型：{content_type}")
        title = body.decode("utf-8").split("<title>", 1)[1].split("</title>", 1)[0]
        print(f"页面标题：{title}")

        status, content_type, body = get("/status")
        print(f"GET /status 状态：{status}，类型：{content_type}")
        print(json.dumps(json.loads(body.decode("utf-8")),
                         ensure_ascii=False, sort_keys=True))
    finally:
        server.shutdown()
        server.server_close()
        thread.join(2)
    print(f"服务线程已退出：{not thread.is_alive()}")


def serve(port):
    try:
        server = LimitedServer((HOST, port), Handler)
    except OSError as error:
        print(f"无法监听端口 {port}：{error.strerror}")
        print("换一个端口，或省略端口让系统分配。")
        return
    host, actual = server.server_address
    print(f"服务地址：http://{host}:{actual}/")
    print("只绑定回环地址；按 Ctrl+C 停止。")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n收到 Ctrl+C，正在停止。")
    finally:
        server.server_close()
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
        server = LimitedServer((HOST, 0), Handler)
        run_self_test(server)
