# 练习 4-11：用同一个种子生成两次噪声，核对结果一致。
import numpy as np


line = np.linspace(0.0, 1.0, 6)
rng = np.random.default_rng(7)
first = line + rng.normal(0.0, 0.05, size=line.size)
rng2 = np.random.default_rng(7)
second = line + rng2.normal(0.0, 0.05, size=line.size)

print("same:", bool(np.allclose(first, second)))
print("mean:", round(float(first.mean()), 3))
print("std:", round(float(first.std()), 3))
print("min/max:", np.round([first.min(), first.max()], 3).tolist())

rng3 = np.random.default_rng(8)
other = line + rng3.normal(0.0, 0.05, size=line.size)
print("different:", bool(not np.allclose(first, other)))
