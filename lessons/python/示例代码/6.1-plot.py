# 用一组平方数画折线，设置标题、坐标标签、范围与图例。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


x = np.arange(6)
y = x ** 2
fig, ax = plt.subplots(figsize=(5, 3), dpi=100)
line, = ax.plot(x, y, color="tab:blue", marker="o", linewidth=2,
                label="y = x^2")
ax.set_title("Square numbers")
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_xlim(0, 5)
ax.set_ylim(0, 25)
ax.legend()
fig.savefig("out_plot_basic.png")
print("figure size:", fig.get_size_inches().tolist())
print("dpi:", int(fig.get_dpi()))
print("axes:", len(fig.axes))
print("line points:", len(line.get_xdata()))
print("legend:", [t.get_text() for t in ax.get_legend().get_texts()])
plt.close(fig)
