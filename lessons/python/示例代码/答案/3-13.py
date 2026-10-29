# 练习 3-13：用小端格式打包一个命令字、长度、总数与浮点系数，再拆开。
import struct

data = struct.pack("<BHIf", 7, 300, 70000, 1.5)
print("size:", len(data), struct.calcsize("<BHIf"))
print("hex:", data.hex())
command, length, total, scale = struct.unpack("<BHIf", data)
print("fields:", command, length, total, scale)
raw = total.to_bytes(4, "little")
print("bytes:", raw.hex())
print("back:", int.from_bytes(raw, "little"))
