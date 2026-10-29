# 练习 9-1：统计单张图片的尺寸、通道、均值与目标数量。
from pathlib import Path

import cv2
import numpy as np

path = Path("素材/synth_shapes.png")
image = cv2.imread(str(path))
if image is None:
    raise SystemExit("cannot read image: %s" % path)
mean = image.mean(axis=(0, 1))
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
mask = ((hsv[:, :, 1] >= 60) & (hsv[:, :, 2] >= 60)).astype(np.uint8) * 255
contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                               cv2.CHAIN_APPROX_SIMPLE)
targets = sum(1 for contour in contours
              if cv2.contourArea(contour) >= 200.0)
print("file:", path.name)
print("size: %dx%d" % (image.shape[1], image.shape[0]))
print("channels:", image.shape[2])
print("mean b,g,r: %.2f,%.2f,%.2f" % (mean[0], mean[1], mean[2]))
print("targets:", targets)
