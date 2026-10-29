# 在同一个坐标轴里画平方与立方两条曲线，并打印线条标签与数据。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


x = np.array([1, 2, 3, 4])
fig, ax = plt.subplots(figsize=(4, 3), dpi=100)
ax.plot(x, x ** 2, label="square")
ax.plot(x, x ** 3, label="cube")
ax.legend()
fig.savefig("out_ex_6_2.png")
print("axes:", len(fig.axes))
print("lines:", len(ax.lines))
print("labels:", [t.get_text() for t in ax.get_legend().get_texts()])
print("cube y:", ax.lines[1].get_ydata().tolist())
plt.close(fig)
