# 构造合成帧，用缓冲区解析器拆出字段，处理两帧相连与半帧。
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
    checksum = frame[3 + length]
    if len(frame) != 4 + length:
        raise ValueError("length mismatch")
    if crc8(frame[1:3 + length]) != checksum:
        raise ValueError("crc mismatch")
    return command, payload


class FrameParser:
    def __init__(self):
        self.buffer = bytearray()

    def feed(self, data):
        self.buffer.extend(data)
        frames = []
        while True:
            start = self.buffer.find(HEADER)
            if start < 0:
                self.buffer.clear()
                break
            if start:
                del self.buffer[:start]
            if len(self.buffer) < 2:
                break
            total = 4 + self.buffer[1]
            if len(self.buffer) < total:
                break
            raw = bytes(self.buffer[:total])
            del self.buffer[:total]
            try:
                frames.append(parse_frame(raw))
            except ValueError as error:
                frames.append(("error", str(error)))
        return frames


frame_a = build_frame(0x01, b"\x0a\x0b")
frame_b = build_frame(0x02, b"\x64")
print("crc:", hex(crc8(b"\x01\x02")))
print("frame:", frame_a.hex())
print("fields:", parse_frame(frame_a))

parser = FrameParser()
print("combined:", parser.feed(frame_a + frame_b))
print("rest:", bytes(parser.buffer).hex())
print("split:", parser.feed(frame_a[:3]))
print("split:", parser.feed(frame_a[3:]))
bad = bytearray(frame_a)
bad[-1] ^= 0xFF
print("noise:", parser.feed(b"\x00" + bytes(bad)))
