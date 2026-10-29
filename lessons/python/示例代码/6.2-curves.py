# 在同一张图里画正弦与余弦，并用两个子图比较阻尼振荡与参考线。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


x = np.linspace(0.0, 2.0 * np.pi, 101)
fig, (left, right) = plt.subplots(1, 2, figsize=(7, 3), dpi=100)
left.plot(x, np.sin(x), color="tab:blue", label="sin")
left.plot(x, np.cos(x), color="tab:orange", linestyle="--", label="cos")
left.set_title("sin and cos")
left.set_xlabel("x")
left.grid(True)
left.legend()
damped = np.exp(-x / 4.0) * np.sin(2.0 * x)
right.plot(x, damped, color="tab:red")
right.axhline(0.0, color="gray", linewidth=0.8)
right.axvline(1.5, color="gray", linestyle=":", linewidth=0.8)
right.text(1.6, 0.75, "x = 1.5")
right.set_title("damped wave")
right.set_xlabel("x")
fig.tight_layout()
fig.savefig("out_plot_curves.png")
print("axes:", len(fig.axes))
print("left lines:", len(left.lines))
print("right lines:", len(right.lines))
print("legend:", [t.get_text() for t in left.get_legend().get_texts()])
print("grid:", bool(left.get_xgridlines()[0].get_visible()))
plt.close(fig)
