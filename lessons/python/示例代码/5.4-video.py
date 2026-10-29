# 逐帧读取视频，按颜色找到移动方块，并写出标注视频与抽帧图。
from pathlib import Path

import cv2
import numpy as np


source = Path("素材/synth_cube.mp4")
cap = cv2.VideoCapture(str(source))
print("opened:", cap.isOpened())
print("size:", int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)),
      int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
print("fps:", cap.get(cv2.CAP_PROP_FPS))

out_dir = Path("out_video")
out_dir.mkdir(exist_ok=True)
writer = cv2.VideoWriter(str(out_dir / "annotated.mp4"),
                         cv2.VideoWriter_fourcc(*"mp4v"), 12.0, (320, 240))
print("writer:", writer.isOpened())

frames = 0
saved = 0
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
        largest = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(largest)
        centers.append((x + w // 2, y + h // 2))
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
    cv2.putText(frame, "frame %02d" % frames, (10, 225),
                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1,
                cv2.LINE_AA)
    writer.write(frame)
    if frames % 6 == 0:
        cv2.imwrite(str(out_dir / ("frame_%03d.png" % frames)), frame)
        saved += 1
    frames += 1

cap.release()
writer.release()
print("frames:", frames, "saved:", saved)
print("center first/last:", centers[0], centers[-1] if centers else None)
print("video saved:", (out_dir / "annotated.mp4").exists())
