# 练习 4-12：生成随机掩码，统计行列覆盖，并抽样与打乱。
import numpy as np


rng = np.random.default_rng(42)
mask = rng.random((6, 8)) < 0.3
print("shape/count:", mask.shape, int(mask.sum()))
print("row counts:", mask.sum(axis=1).tolist())
print("coverage:", round(float(mask.mean()), 3))

choices = rng.choice(np.arange(5), size=6, replace=True)
print("choices:", choices.tolist())
order = np.arange(6)
rng.shuffle(order)
print("shuffle:", order.tolist())
print("ints:", rng.integers(10, 20, size=(2, 3)).tolist())
