# 保存一张四格图，比较固定边界与紧凑边界的像素尺寸。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import image as mpimg


x = np.linspace(0.0, 4.0, 121)
fig, axes = plt.subplots(2, 2, figsize=(6, 4), dpi=100)
axes[0, 0].plot(x, np.sin(x))
axes[0, 0].set_title("sin")
axes[0, 1].plot(x, np.cos(x))
axes[0, 1].set_title("cos")
axes[1, 0].plot(x, np.exp(-x))
axes[1, 0].set_title("exp")
axes[1, 1].plot(x, x)
axes[1, 1].set_title("linear")
fig.tight_layout()
fig.savefig("out_plot_report.png", dpi=120, bbox_inches="tight",
            facecolor="white")
fig.savefig("out_plot_report_plain.png", dpi=120, facecolor="white")
tight = mpimg.imread("out_plot_report.png")
plain = mpimg.imread("out_plot_report_plain.png")
print("axes:", len(fig.axes))
print("size inches:", fig.get_size_inches().tolist())
print("tight pixels:", tight.shape)
print("plain pixels:", plain.shape)
print("dtype:", tight.dtype)
print("max value:", float(tight.max()))
plt.close(fig)
