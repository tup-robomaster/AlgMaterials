# 用固定分数画直方图，并打印每个区间的计数与边界。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


scores = np.array([52, 61, 64, 68, 72, 75, 79, 83, 88, 95])
fig, ax = plt.subplots(figsize=(4, 3), dpi=100)
counts, edges, _ = ax.hist(scores, bins=5, range=(50, 100),
                           color="tab:purple", edgecolor="white")
ax.set_title("Score histogram")
ax.set_xlabel("score")
ax.set_ylabel("count")
fig.savefig("out_ex_6_6.png")
print("counts:", counts.astype(int).tolist())
print("edges:", edges.tolist())
print("total:", int(counts.sum()))
plt.close(fig)
