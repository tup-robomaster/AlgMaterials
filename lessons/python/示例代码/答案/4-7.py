# 练习 4-7：统计彩色图的通道均值与最大值，并定位最亮像素。
import cv2
import numpy as np


image = cv2.imread("素材/synth_balls.png")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
print("mean:", np.round(image.mean(axis=(0, 1)), 2).tolist())
print("max:", image.max(axis=(0, 1)).tolist())
print("std:", np.round(image.std(axis=(0, 1)), 2).tolist())

flat_index = int(np.argmax(gray))
y, x = np.unravel_index(flat_index, gray.shape)
print("brightest:", x, y, int(gray[y, x]))
print("over 200:", int((gray > 200).sum()))
print("percentile 50/90:", np.percentile(gray, [50, 90]).astype(int).tolist())
