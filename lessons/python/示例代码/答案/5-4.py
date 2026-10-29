# 练习 5-4：比较 Otsu 与自适应阈值，并用掩码取出绿色像素。
import cv2
import numpy as np


grid = cv2.imread("素材/synth_grid.png", cv2.IMREAD_GRAYSCALE)
value, otsu = cv2.threshold(grid, 0, 255,
                            cv2.THRESH_BINARY + cv2.THRESH_OTSU)
print("otsu:", int(value), int(otsu.sum() // 255))

adaptive = cv2.adaptiveThreshold(grid, 255, cv2.ADAPTIVE_THRESH_MEAN_C,
                                 cv2.THRESH_BINARY, 11, 2)
print("adaptive:", int(adaptive.sum() // 255))

image = cv2.imread("素材/synth_balls.png")
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
mask = cv2.inRange(hsv, np.array([35, 80, 80]), np.array([85, 255, 255]))
green = cv2.bitwise_and(image, image, mask=mask)
print("green mean:", np.round(green[mask > 0].mean(axis=0), 2).tolist())
print("where count:", int(np.where(mask > 0, 1, 0).sum()))
