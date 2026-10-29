# 把同一张图按 72 与 144 两种分辨率保存，比较像素尺寸。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import image as mpimg


x = np.linspace(0.0, 1.0, 40)
fig, ax = plt.subplots(figsize=(4, 2), dpi=100)
ax.plot(x, x ** 2)
fig.savefig("out_ex_6_7_72.png", dpi=72)
fig.savefig("out_ex_6_7_144.png", dpi=144)
small = mpimg.imread("out_ex_6_7_72.png")
large = mpimg.imread("out_ex_6_7_144.png")
print("72:", small.shape)
print("144:", large.shape)
print("double:", large.shape[:2] == tuple(2 * n for n in small.shape[:2]))
plt.close(fig)
