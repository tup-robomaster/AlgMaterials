# 练习 5-3：在 HSV 中取绿色圆球，统计像素、覆盖率与连通区域。
import cv2
import numpy as np


image = cv2.imread("素材/synth_balls.png")
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
low = np.array([35, 80, 80], np.uint8)
high = np.array([85, 255, 255], np.uint8)
mask = cv2.inRange(hsv, low, high)

print("green pixels:", int(mask.sum() // 255))
print("coverage:", round(float(mask.mean() / 255.0), 4))
count, labels, stats, centers = cv2.connectedComponentsWithStats(mask)
print("components:", count - 1)
for index in range(1, count):
    x, y, w, h, area = stats[index]
    print("region:", int(area), (int(x), int(y), int(w), int(h)))
print("mask saved:", bool(cv2.imwrite("out_green_mask.png", mask)))
