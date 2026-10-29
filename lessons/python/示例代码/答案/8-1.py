# 练习 8-1：每隔 10 帧取一次重心，最后补上末帧。
import cv2
import numpy as np

cap = cv2.VideoCapture("素材/synth_cube.mp4")
centers = {}
index = 0
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
            centers[index] = (moments["m10"] / moments["m00"],
                              moments["m01"] / moments["m00"])
    index += 1
cap.release()

for frame_no in (0, 10, 20, 30, 35):
    x, y = centers[frame_no]
    print("frame %d: x=%.1f y=%.1f" % (frame_no, x, y))
