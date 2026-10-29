# 用四组数据画一条红色虚线，并打印画布与坐标轴信息。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


fig, ax = plt.subplots(figsize=(4, 2), dpi=80)
line, = ax.plot([1, 2, 3, 4], [2, 4, 8, 16],
                color="red", linestyle="--", label="growth")
ax.set_xlim(1.0, 4.0)
ax.set_ylim(0.0, 16.0)
ax.legend()
fig.savefig("out_ex_6_1.png")
print("size:", fig.get_size_inches().tolist())
print("dpi:", int(fig.get_dpi()))
print("axes:", len(fig.axes))
print("color/style:", line.get_color(), line.get_linestyle())
print("xlim:", ax.get_xlim())
print("legend:", [t.get_text() for t in ax.get_legend().get_texts()])
plt.close(fig)
