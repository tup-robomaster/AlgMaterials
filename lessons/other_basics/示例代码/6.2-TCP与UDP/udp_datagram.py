# UDP 保留数据报边界：两次发送对应两次接收。
import socket
import threading

ready = threading.Event()
result = {"address": None, "datagrams": []}


def serve_once():
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(("127.0.0.1", 0))
    result["address"] = server.getsockname()
    ready.set()
    for _ in range(2):
        data, peer = server.recvfrom(1024)
        result["datagrams"].append(data)
    server.close()


thread = threading.Thread(target=serve_once, daemon=True)
thread.start()
if not ready.wait(2):
    raise SystemExit("服务端没有在 2 秒内启动")

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as client:
    client.sendto(b"10,20", result["address"])
    client.sendto(b"30,40", result["address"])
    print("客户端：两次 sendto 发送两个数据报")

thread.join(2)
for number, data in enumerate(result["datagrams"], 1):
    print(f"服务端第 {number} 个数据报：{data.decode()}")
