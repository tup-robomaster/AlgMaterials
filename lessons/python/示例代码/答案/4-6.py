# 练习 4-6：拼接上下两半图像，用广播求平均并统计差值。
import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_GRAYSCALE)
top = image[:240]
bottom = image[240:]
joined = np.concatenate([top, bottom], axis=1)
print("joined:", joined.shape)

shifted = np.roll(image, 1, axis=1).astype(np.float32)
blur = (image.astype(np.float32) + shifted) / 2.0
print("blur mean:", round(float(blur.mean()), 2))

diff = np.abs(image.astype(np.int16) - blur.astype(np.int16))
print("diff max/mean:", int(diff.max()), round(float(diff.mean()), 2))
print("stack:", np.stack([top, bottom], axis=0).shape)
