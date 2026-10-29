# 在一张灰度图上练习切片、掩码、花式索引与视图修改。
import cv2
import numpy as np


image = cv2.imread("素材/synth_grid.png", cv2.IMREAD_GRAYSCALE)
print("shape:", image.shape, image.dtype)
print("corner:", int(image[0, 0]), int(image[-1, -1]))

roi = image[40:120, 60:180]
print("roi:", roi.shape, int(roi.mean()))

view = image[0:40, 0:60]
view[:] = 255
print("view changed parent:", int(image[0, 0]), int(image[40, 0]))

copied = image[0:40, 0:60].copy()
copied[:] = 0
print("copy changed parent:", int(image[0, 0]))

mask = image > 200
ys, xs = np.where(mask)
print("mask:", mask.shape, int(mask.sum()), round(float(mask.mean()), 4))
print("first white:", int(xs[0]), int(ys[0]))
print("where sum:", int(np.where(mask, 1, 0).sum()))

fancy = image[[40, 60, 80], [0, 40, 80]]
print("fancy:", fancy.tolist())
print("take:", np.take(image, [0, 10, 20], axis=0).shape)
print("newaxis:", image[..., np.newaxis].shape)
