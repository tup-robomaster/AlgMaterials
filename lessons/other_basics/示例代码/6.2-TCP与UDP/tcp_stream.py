# TCP 是字节流：一次发送的两行，接收端要按换行自己拆开。
import socket
import threading

ready = threading.Event()
result = {"address": None, "lines": []}


def serve_once():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", 0))
    server.listen(1)
    result["address"] = server.getsockname()
    ready.set()
    conn, peer = server.accept()
    with conn, conn.makefile("rb") as stream:
        for _ in range(2):
            result["lines"].append(stream.readline())
    server.close()


thread = threading.Thread(target=serve_once, daemon=True)
thread.start()
if not ready.wait(2):
    raise SystemExit("服务端没有在 2 秒内启动")

with socket.create_connection(result["address"], timeout=2) as client:
    client.sendall(b"10,20\n30,40\n")
    print("客户端：一次 sendall 发送两行")

thread.join(2)
for number, line in enumerate(result["lines"], 1):
    print(f"服务端第 {number} 行：{line.decode().rstrip()}")
