# 练习 5-5：阈值化后开运算，按面积筛轮廓并打印外接矩形。
import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
_, binary = cv2.threshold(gray, 120, 255, cv2.THRESH_BINARY_INV)
kernel = np.ones((3, 3), np.uint8)
opened = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)

contours, _ = cv2.findContours(opened, cv2.RETR_EXTERNAL,
                               cv2.CHAIN_APPROX_SIMPLE)
kept = [contour for contour in contours if cv2.contourArea(contour) > 100]
kept.sort(key=cv2.contourArea, reverse=True)
print("contours:", len(contours), "kept:", len(kept))
for contour in kept:
    x, y, w, h = cv2.boundingRect(contour)
    print("bbox:", (x, y, w, h), "area:", int(cv2.contourArea(contour)))
