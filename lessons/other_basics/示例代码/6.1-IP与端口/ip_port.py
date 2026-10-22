# 在回环地址上建立一次 TCP 连接，观察 IP 地址与端口。
import socket
import threading

ready = threading.Event()
result = {}


def serve_once():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", 0))
    server.listen(1)
    result["address"] = server.getsockname()
    ready.set()
    conn, peer = server.accept()
    with conn:
        result["request"] = conn.recv(1024)
        conn.sendall(b"pong\n")
    server.close()


thread = threading.Thread(target=serve_once, daemon=True)
thread.start()
if not ready.wait(2):
    raise SystemExit("服务端没有在 2 秒内启动")

host, port = result["address"]
print(f"监听地址：{host}:{port}")

with socket.create_connection((host, port), timeout=2) as client:
    local_host, local_port = client.getsockname()
    peer_host, peer_port = client.getpeername()
    print(f"客户端本地地址：{local_host}:{local_port}")
    print(f"客户端连接的对端地址：{peer_host}:{peer_port}")
    client.sendall(b"ping\n")
    reply = client.recv(1024)

thread.join(2)
print(f"服务端收到：{result['request'].decode().strip()}")
print(f"客户端收到：{reply.decode().strip()}")
