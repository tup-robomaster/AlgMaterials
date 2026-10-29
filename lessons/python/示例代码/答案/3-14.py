# 练习 3-14：把一帧拆成三段送入缓冲区，逐段观察等待与解析结果。
HEADER = 0xA5


def crc8(data):
    value = 0
    for byte in data:
        value ^= byte
        for _ in range(8):
            if value & 0x80:
                value = ((value << 1) ^ 0x07) & 0xFF
            else:
                value = (value << 1) & 0xFF
    return value


def build_frame(command, payload):
    body = bytes([len(payload), command]) + payload
    return bytes([HEADER]) + body + bytes([crc8(body)])


def parse_frame(frame):
    length = frame[1]
    command = frame[2]
    payload = frame[3:3 + length]
    return command, payload


buffer = bytearray()
frame = build_frame(0x02, b"\x01\x02\x03")

for part in (frame[:2], frame[2:5], frame[5:]):
    buffer.extend(part)
    total = 4 + buffer[1]
    if len(buffer) >= total:
        raw = bytes(buffer[:total])
        del buffer[:total]
        print(parse_frame(raw))
        print("crc ok:", crc8(raw[1:-1]) == raw[-1])
    else:
        print("waiting:", len(buffer))
