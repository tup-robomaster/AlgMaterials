# 练习 9-6：读视频、按颜色与面积筛选目标，保存抽帧图与标注视频。
from pathlib import Path

import cv2
import numpy as np

SOURCE = "素材/synth_cube.mp4"
OUT_DIR = "out_9_6_frames"
cap = cv2.VideoCapture(SOURCE)
if not cap.isOpened():
    raise SystemExit("cannot open video: %s" % SOURCE)
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = cap.get(cv2.CAP_PROP_FPS)
writer = cv2.VideoWriter("out_9_6.mp4", cv2.VideoWriter_fourcc(*"mp4v"),
                         fps, (width, height))
if not writer.isOpened():
    raise SystemExit("cannot open video writer")
Path(OUT_DIR).mkdir(exist_ok=True)
index = 0
detected = 0
saved = 0
while True:
    ok, frame = cap.read()
    if not ok:
        break
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, np.array([5, 100, 100]),
                       np.array([35, 255, 255]))
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    best = max(contours, key=cv2.contourArea, default=None)
    if best is not None and cv2.contourArea(best) >= 1000.0:
        detected += 1
        x, y, w, h = cv2.boundingRect(best)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
    if index in (0, 12, 24):
        cv2.imwrite("%s/frame_%03d.png" % (OUT_DIR, index), frame)
        saved += 1
    writer.write(frame)
    index += 1
cap.release()
writer.release()
print("frames:", index)
print("detected:", detected)
print("saved frames:", saved)
print("video: out_9_6.mp4")
