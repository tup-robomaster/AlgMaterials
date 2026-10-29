# 按三种标志读图，查看形状与通道，再拆分、合并并保存结果。
import cv2
import numpy as np


color = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_COLOR)
gray = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_GRAYSCALE)
unchanged = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_UNCHANGED)
print("color:", color.shape, color.dtype)
print("gray:", gray.shape, gray.dtype)
print("unchanged:", unchanged.shape)
print("pixel BGR:", color[100, 100].tolist())
print("gray pixel:", int(gray[100, 100]))

b, g, r = cv2.split(color)
print("channel means:", round(float(b.mean()), 2),
      round(float(g.mean()), 2), round(float(r.mean()), 2))
merged = cv2.merge([b, g, r])
swapped = cv2.merge([r, g, b])
print("merge equal:", bool(np.array_equal(merged, color)))
print("swap equal:", bool(np.array_equal(swapped, color)))

roi = color[100:220, 80:260]
print("roi:", roi.shape)
print("gray saved:", bool(cv2.imwrite("out_gray.png", gray)))
print("roi saved:", bool(cv2.imwrite("out_roi.png", roi)))
