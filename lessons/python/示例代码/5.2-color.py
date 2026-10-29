# 在 HSV 空间取红色双区间，统计掩码与连通区域，再比较 Otsu。
import cv2
import numpy as np


image = cv2.imread("素材/synth_balls.png")
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
low1 = np.array([0, 120, 80], np.uint8)
high1 = np.array([10, 255, 255], np.uint8)
low2 = np.array([170, 120, 80], np.uint8)
high2 = np.array([179, 255, 255], np.uint8)
mask = cv2.inRange(hsv, low1, high1) | cv2.inRange(hsv, low2, high2)

pixels = int(mask.sum() // 255)
print("red pixels:", pixels)
print("coverage:", round(float(mask.mean() / 255.0), 4))

count, labels, stats, centers = cv2.connectedComponentsWithStats(mask)
print("components:", count - 1)
for index in range(1, count):
    x, y, w, h, area = stats[index]
    cx, cy = centers[index]
    print("region:", int(area), (int(x), int(y), int(w), int(h)),
          (int(cx), int(cy)))

red = cv2.bitwise_and(image, image, mask=mask)
print("masked mean:", np.round(red[mask > 0].mean(axis=0), 2).tolist())
print("mask saved:", bool(cv2.imwrite("out_red_mask.png", mask)))

grid = cv2.imread("素材/synth_grid.png", cv2.IMREAD_GRAYSCALE)
value, binary = cv2.threshold(grid, 0, 255,
                              cv2.THRESH_BINARY + cv2.THRESH_OTSU)
print("otsu value:", int(value), "white:", int(binary.sum() // 255))
adaptive = cv2.adaptiveThreshold(grid, 255, cv2.ADAPTIVE_THRESH_MEAN_C,
                                 cv2.THRESH_BINARY, 11, 2)
print("adaptive white:", int(adaptive.sum() // 255))
