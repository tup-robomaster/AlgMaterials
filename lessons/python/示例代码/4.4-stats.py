# 按通道与整体统计图像，再查看排序、唯一值与分位数。
import cv2
import numpy as np


image = cv2.imread("素材/synth_balls.png")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

print("channel mean:", np.round(image.mean(axis=(0, 1)), 2).tolist())
print("channel std:", np.round(image.std(axis=(0, 1)), 2).tolist())
print("channel max:", image.max(axis=(0, 1)).tolist())
print("channel min:", image.min(axis=(0, 1)).tolist())
print("gray mean/std:", round(float(gray.mean()), 2), round(float(gray.std()), 2))

flat_index = int(np.argmax(gray))
y, x = np.unravel_index(flat_index, gray.shape)
print("brightest:", int(gray[y, x]), int(x), int(y))
print("percentiles:", np.percentile(gray, [10, 50, 90]).astype(int).tolist())

hist, edges = np.histogram(gray, bins=4, range=(0, 256))
print("hist:", hist.tolist())
print("edges:", edges.astype(int).tolist())

values = np.array([3, 1, 2, 3, 5, 1])
print("sort:", np.sort(values).tolist())
print("argsort:", np.argsort(values, kind="stable").tolist())
unique, counts = np.unique(values, return_counts=True)
print("unique:", unique.tolist(), counts.tolist())

data = np.array([1.0, np.nan, 3.0])
print("nan:", float(np.nanmean(data)), float(np.nanstd(data)))
