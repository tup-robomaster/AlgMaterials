# 扫描视频元数据与目标重心，为参数表提供依据。
# 只用颜色掩码与最大轮廓，不做跟踪，也不写结果文件。
from pathlib import Path

import cv2
import numpy as np


SOURCE = Path("素材/synth_cube.mp4")
LOWER = np.array([5, 100, 100])
UPPER = np.array([35, 255, 255])
KERNEL = np.ones((3, 3), np.uint8)
LINE = 180.0

cap = cv2.VideoCapture(str(SOURCE))
if not cap.isOpened():
    raise SystemExit("cannot open video: %s" % SOURCE)
frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = cap.get(cv2.CAP_PROP_FPS)

centers = []
areas = []
index = 0
while True:
    ok, frame = cap.read()
    if not ok:
        break
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, LOWER, UPPER)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    if contours:
        contour = max(contours, key=cv2.contourArea)
        moments = cv2.moments(contour)
        if moments["m00"]:
            cx = moments["m10"] / moments["m00"]
            cy = moments["m01"] / moments["m00"]
            centers.append((index, cx, cy))
            areas.append(cv2.contourArea(contour))
    index += 1
cap.release()

if not centers:
    raise SystemExit("no target found in %s" % SOURCE)
crossings = [now[0] for now, before in zip(centers[1:], centers[:-1])
             if before[1] < LINE <= now[1]]
print("frames:", frames)
print("size: %dx%d" % (width, height))
print("fps:", fps)
print("first center: (%.1f, %.1f)" % centers[0][1:])
print("last center: (%.1f, %.1f)" % centers[-1][1:])
print("area: %.0f..%.0f" % (min(areas), max(areas)))
if crossings:
    print("line %.1f: %d crossing at frame %d"
          % (LINE, len(crossings), crossings[0]))
else:
    print("line %.1f: 0 crossings" % LINE)
