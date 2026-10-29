# 用固定种子生成 80 个点，画散点图并打印数量与横轴范围。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


rng = np.random.default_rng(3)
x = rng.uniform(0.0, 5.0, 80)
y = 2.0 * x + rng.normal(0.0, 1.0, 80)
fig, ax = plt.subplots(figsize=(4, 3), dpi=100)
ax.scatter(x, y, s=20, color="tab:green")
ax.set_title("Generated points")
ax.set_xlabel("x")
ax.set_ylabel("y")
fig.savefig("out_ex_6_5.png")
print("points:", len(x))
print("collections:", len(ax.collections))
print("x range:", round(float(x.min()), 2), round(float(x.max()), 2))
plt.close(fig)
