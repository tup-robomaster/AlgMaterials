# 练习 5-2：拆分 BGR 通道，交换顺序后合并，并核对像素变化。
import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_COLOR)
b, g, r = cv2.split(image)
merged = cv2.merge([b, g, r])
swapped = cv2.merge([r, g, b])

print("merge equal:", bool(np.array_equal(merged, image)))
print("swap equal:", bool(np.array_equal(swapped, image)))
print("original:", image[100, 100].tolist())
print("swapped:", swapped[100, 100].tolist())
print("means B/G/R:", round(float(b.mean()), 2),
      round(float(g.mean()), 2), round(float(r.mean()), 2))
print("shape:", image.shape)
