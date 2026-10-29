# 练习 4-1：创建三种数组并打印形状、步长、字节数与内存顺序。
import numpy as np


a = np.arange(12, dtype=np.uint8).reshape(3, 4)
b = np.linspace(-1.0, 1.0, 5)
c = np.full((2, 3), 5, dtype=np.int16, order="F")

print("a:", a.shape, a.ndim, a.dtype, a.size)
print("a strides:", a.strides)
print("b:", b.tolist())
print("c flags:", c.flags["C_CONTIGUOUS"], c.flags["F_CONTIGUOUS"])
print("bytes:", a.nbytes, b.nbytes, c.nbytes)
