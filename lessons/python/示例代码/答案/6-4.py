# 为两条曲线设置不同线型与标记，并打印设置结果。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


x = [0, 1, 2, 3]
fig, ax = plt.subplots(figsize=(4, 2.5), dpi=100)
square, = ax.plot(x, [0, 1, 4, 9], linestyle="-", marker="o",
                  label="square")
cube, = ax.plot(x, [0, 1, 8, 27], linestyle=":", marker="s",
                label="cube")
ax.legend()
fig.savefig("out_ex_6_4.png")
print("lines:", len(ax.lines))
print("styles:", square.get_linestyle(), cube.get_linestyle())
print("markers:", square.get_marker(), cube.get_marker())
print("legend:", [t.get_text() for t in ax.get_legend().get_texts()])
plt.close(fig)
