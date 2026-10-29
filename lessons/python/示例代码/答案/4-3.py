# 练习 4-3：裁剪棋盘 ROI，统计亮像素，并用花式索引取三行。
import cv2
import numpy as np


gray = cv2.imread("素材/synth_grid.png", cv2.IMREAD_GRAYSCALE)
roi = gray[100:180, 200:360]
print("roi:", roi.shape, int(roi.min()), int(roi.max()))

mask = gray >= 200
print("white:", int(mask.sum()), round(float(mask.mean()), 3))
print("rows:", gray[[0, 20], 0:41:20].tolist())
ys, xs = np.where(gray < 50)
print("dark first:", int(xs[0]), int(ys[0]))
