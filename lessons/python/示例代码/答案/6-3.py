# 用两个子图分别画正弦与余弦，并打印标题、线条数与图例。
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


x = np.linspace(0.0, np.pi, 50)
fig, (left, right) = plt.subplots(1, 2, figsize=(6, 2.5), dpi=100)
left.plot(x, np.sin(x), label="sin")
left.set_title("Sine")
left.legend()
right.plot(x, np.cos(x), label="cos")
right.set_title("Cosine")
right.legend()
fig.tight_layout()
fig.savefig("out_ex_6_3.png")
print("axes:", len(fig.axes))
print("titles:", left.get_title(), right.get_title())
print("lines:", len(left.lines), len(right.lines))
print("legend:", [t.get_text() for t in right.get_legend().get_texts()])
plt.close(fig)
