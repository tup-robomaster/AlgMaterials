# 练习 8-2：把四条竖线的首次穿越帧列成表，末条线没有穿越。
import cv2
import numpy as np

LINES = (60.0, 100.0, 180.0, 240.0, 260.0)
cap = cv2.VideoCapture("素材/synth_cube.mp4")
centers = []
while True:
    ok, frame = cap.read()
    if not ok:
        break
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, np.array([5, 100, 100]),
                       np.array([35, 255, 255]))
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    if contours:
        contour = max(contours, key=cv2.contourArea)
        moments = cv2.moments(contour)
        if moments["m00"]:
            centers.append(moments["m10"] / moments["m00"])
cap.release()

for line in LINES:
    frames = [index for index in range(1, len(centers))
              if centers[index - 1] < line <= centers[index]]
    if frames:
        print("line %.1f: frame %d" % (line, frames[0]))
    else:
        print("line %.1f: none" % line)
