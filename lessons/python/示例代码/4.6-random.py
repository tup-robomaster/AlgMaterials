# 用固定种子生成带噪声的直线、随机掩码与可复现的抽样。
import numpy as np


rng = np.random.default_rng(2026)
x = np.linspace(0.0, 4.0, 9)
noise = rng.normal(0.0, 0.15, size=x.size)
y = 2.0 * x + 1.0 + noise
print("first y:", np.round(y[:3], 3).tolist())
print("noise mean:", round(float(noise.mean()), 3))

design = np.column_stack([x, np.ones_like(x)])
coef, _, _, _ = np.linalg.lstsq(design, y, rcond=None)
print("fit:", np.round(coef, 3).tolist())
print("max error:", round(float(np.max(np.abs(design @ coef - y))), 3))

mask = rng.random((4, 5)) < 0.5
print("mask:", mask.shape, int(mask.sum()))
print("ints:", rng.integers(0, 10, size=5).tolist())
print("choice:", rng.choice(["a", "b", "c"], size=3, replace=False).tolist())

names = np.array(["a", "b", "c", "d"])
rng.shuffle(names)
print("shuffle:", names.tolist())

again = np.random.default_rng(2026)
repeat = again.normal(0.0, 0.15, size=x.size)
print("reproducible:", bool(np.allclose(repeat, noise)))

points = np.column_stack([x, y])
np.savetxt("out_points.csv", points, delimiter=",", fmt="%.3f",
           header="x,y", comments="")
print("saved rows:", points.shape[0])
