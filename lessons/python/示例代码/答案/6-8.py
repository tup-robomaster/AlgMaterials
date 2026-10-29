# 用紧凑边界和浅灰底色保存两个子图，并读回检查像素。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import image as mpimg


fig, axes = plt.subplots(1, 2, figsize=(5, 2), dpi=110)
axes[0].plot([0, 1, 2], [0, 1, 4])
axes[0].set_title("square")
axes[1].plot([0, 1, 2], [0, 1, 8])
axes[1].set_title("cube")
fig.tight_layout()
fig.savefig("out_ex_6_8.png", bbox_inches="tight", facecolor="#f0f0f0")
pixels = mpimg.imread("out_ex_6_8.png")
print("axes:", len(fig.axes))
print("shape:", pixels.shape)
print("dtype:", pixels.dtype)
print("size inches:", fig.get_size_inches().tolist())
plt.close(fig)
