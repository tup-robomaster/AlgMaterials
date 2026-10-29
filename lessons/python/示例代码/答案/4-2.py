# 练习 4-2：观察 uint8 加法回绕、astype 扩宽与 int8 溢出。
import numpy as np


x = np.array([250, 2], dtype=np.uint8)
y = np.array([10, 3], dtype=np.uint8)
print("sum:", (x + y).tolist())
print("cast:", x.astype(np.int16).tolist())
print("back:", x.astype(np.int16).astype(np.uint8).tolist())

v = np.array([120], dtype=np.int8)
print("wrap:", (v + 20).tolist())
print("float equal:", bool(0.1 + 0.2 == 0.3))
print("float close:", bool(np.isclose(0.1 + 0.2, 0.3)))
