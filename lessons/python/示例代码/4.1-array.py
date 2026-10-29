# 用多种方式创建数组，查看形状、字节数、步长与顺序。
import numpy as np


a = np.array([[1, 2, 3], [4, 5, 6]], dtype=np.int16)
z = np.zeros((2, 4), dtype=np.uint8)
o = np.ones((3,), dtype=np.float32)
f = np.full((2, 2), 7, dtype=np.int32)
r = np.arange(0, 10, 3)
l = np.linspace(0.0, 1.0, 5)

print("a:", a.shape, a.ndim, a.dtype, a.size, a.itemsize)
print("a strides:", a.strides)
print("C flags:", a.flags["C_CONTIGUOUS"], a.flags["F_CONTIGUOUS"])
b = np.array([[1, 2, 3], [4, 5, 6]], dtype=np.int16, order="F")
print("F flags:", b.flags["C_CONTIGUOUS"], b.flags["F_CONTIGUOUS"])
print("F strides:", b.strides)
print("zeros:", z.shape, z.dtype, int(z.sum()))
print("ones:", o.shape, o.dtype, float(o.sum()))
print("full:", f.shape, int(f[0, 0]))
print("arange:", r.tolist())
print("linspace:", np.round(l, 2).tolist())

small = a.astype(np.int8)
print("astype:", small.dtype, small.nbytes, a.nbytes)
wrap = np.array([120, 100], dtype=np.int8) + np.array([20, 30], dtype=np.int8)
print("wrap:", wrap.tolist())
