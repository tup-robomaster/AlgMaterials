# 练习 5-10：从回退视频中每五帧存一张图，并打印保存数量。
from pathlib import Path

import cv2


cap = cv2.VideoCapture("素材/synth_cube.mp4")
out_dir = Path("out_camera")
out_dir.mkdir(exist_ok=True)

index = 0
saved = []
while True:
    ok, frame = cap.read()
    if not ok:
        break
    if index % 5 == 0:
        path = out_dir / ("frame_%03d.png" % index)
        cv2.imwrite(str(path), frame)
        saved.append(path.name)
    index += 1
cap.release()

print("frames:", index)
print("saved:", len(saved))
print("first/last:", saved[0], saved[-1])
