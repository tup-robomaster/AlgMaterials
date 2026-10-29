# 练习 4-4：比较视图与副本，用 take、布尔掩码和 newaxis 取子集。
import numpy as np


base = np.arange(20).reshape(4, 5)
view = base[1:3, 2:4]
view += 100
print("base sum:", int(base.sum()))

copied = base.copy()
copied[:] = 0
print("copy sum:", int(copied.sum()), "base sum:", int(base.sum()))
print("take:", np.take(base, [0, 3], axis=0).tolist())

mask = base % 2 == 0
print("even:", int(mask.sum()))
selected = np.where(mask, base, -1)
print("where row0:", selected[0].tolist())
print("newaxis:", base[:, 0][:, np.newaxis].shape)
