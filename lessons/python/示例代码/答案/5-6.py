# 练习 5-6：Canny 加闭运算，按圆形度筛出圆球轮廓。
import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray, 50, 150)
kernel = np.ones((3, 3), np.uint8)
closed = cv2.morphologyEx(edges, cv2.MORPH_CLOSE, kernel)

contours, _ = cv2.findContours(closed, cv2.RETR_EXTERNAL,
                               cv2.CHAIN_APPROX_SIMPLE)
rounds = []
for contour in contours:
    area = cv2.contourArea(contour)
    perimeter = cv2.arcLength(contour, True)
    if area < 100 or perimeter == 0:
        continue
    circularity = 4.0 * np.pi * area / (perimeter * perimeter)
    if circularity > 0.8:
        x, y, w, h = cv2.boundingRect(contour)
        rounds.append((circularity, x + w // 2, y + h // 2))

print("all:", len(contours), "round:", len(rounds))
for circularity, cx, cy in rounds:
    print("circle:", round(float(circularity), 2), (cx, cy))
