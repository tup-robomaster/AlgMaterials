# 练习 5-14：比较 ROI 视图与副本，打印共享内存与连续性。
import time

import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_GRAYSCALE)
view = image[100:300, 100:300]
copied = view.copy()

print("view shares:", bool(np.shares_memory(view, image)))
print("copy shares:", bool(np.shares_memory(copied, image)))
print("view contiguous:", bool(view.flags["C_CONTIGUOUS"]))
print("copy contiguous:", bool(copied.flags["C_CONTIGUOUS"]))
print("same sum:", int(view.sum()) == int(copied.sum()))

times = []
for _ in range(5):
    start = time.perf_counter()
    view.sum()
    copied.sum()
    times.append(time.perf_counter() - start)
print("timed pairs:", len(times))
