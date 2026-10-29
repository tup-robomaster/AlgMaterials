# 用 Canny 与闭运算提取边缘，再按面积筛轮廓并画出外框。
import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray, 50, 150)
kernel = np.ones((3, 3), np.uint8)
closed = cv2.morphologyEx(edges, cv2.MORPH_CLOSE, kernel)

contours, _ = cv2.findContours(closed, cv2.RETR_EXTERNAL,
                               cv2.CHAIN_APPROX_SIMPLE)
kept = [contour for contour in contours if cv2.contourArea(contour) > 100]
kept.sort(key=cv2.contourArea, reverse=True)
print("contours:", len(contours), "kept:", len(kept))

canvas = image.copy()
for contour in kept:
    area = cv2.contourArea(contour)
    perimeter = cv2.arcLength(contour, True)
    circularity = 4.0 * np.pi * area / (perimeter * perimeter)
    x, y, w, h = cv2.boundingRect(contour)
    print("area/bbox:", int(area), (x, y, w, h), round(circularity, 2))
    cv2.rectangle(canvas, (x, y), (x + w, y + h), (0, 255, 255), 2)

cv2.drawContours(canvas, kept, -1, (0, 0, 255), 2)
print("draw saved:", bool(cv2.imwrite("out_contours.png", canvas)))

rect = cv2.minAreaRect(kept[0])
print("min rect size:", tuple(int(round(v)) for v in rect[1]))
print("min rect angle:", round(float(rect[2]), 1))
