# 练习 9-5：用 NumPy 完成裁剪、归一化、通道均值与亮度掩码。
from pathlib import Path

import cv2
import numpy as np

path = Path("素材/synth_shapes.png")
image = cv2.imread(str(path))
if image is None:
    raise SystemExit("cannot read image: %s" % path)
crop = image[60:300, 120:520]
mean = crop.mean(axis=(0, 1))
low = float(crop.min())
high = float(crop.max())
normalized = (crop.astype(np.float32) - low) / (high - low)
mask = (crop.max(axis=2) > 180).astype(np.uint8) * 255
if not cv2.imwrite("out_9_5_mask.png", mask):
    raise SystemExit("cannot write mask")
print("crop: %dx%d" % (crop.shape[1], crop.shape[0]))
print("mean b,g,r: %.2f,%.2f,%.2f" % (mean[0], mean[1], mean[2]))
print("norm min,max: %.2f,%.2f" % (float(normalized.min()),
                                   float(normalized.max())))
print("mask pixels:", int(np.count_nonzero(mask)))
print("saved: out_9_5_mask.png")
