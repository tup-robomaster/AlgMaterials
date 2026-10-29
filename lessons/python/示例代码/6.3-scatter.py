# 用固定随机种子生成学习时长与分数，画散点图与分数直方图。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


rng = np.random.default_rng(7)
hours = rng.uniform(0.0, 10.0, 120)
scores = 55.0 + 4.2 * hours + rng.normal(0.0, 4.0, 120)
fig, (left, right) = plt.subplots(1, 2, figsize=(8, 3), dpi=100)
points = left.scatter(hours, scores, s=18, c=scores, cmap="viridis",
                      alpha=0.8)
left.set_title("Hours vs score")
left.set_xlabel("hours")
left.set_ylabel("score")
left.grid(True)
fig.colorbar(points, ax=left, label="score")
counts, edges, _ = right.hist(scores, bins=8, range=(40.0, 100.0),
                              color="tab:orange", edgecolor="white")
right.set_title("Score distribution")
right.set_xlabel("score")
right.set_ylabel("count")
fig.tight_layout()
fig.savefig("out_plot_scatter.png")
print("points:", len(hours))
print("collections:", len(left.collections))
print("counts:", counts.astype(int).tolist())
print("edges:", np.round(edges, 1).tolist())
print("bins total:", int(counts.sum()))
plt.close(fig)
