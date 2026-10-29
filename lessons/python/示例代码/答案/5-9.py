# 练习 5-9：带 --no-camera 时直接打开课程视频，打印元数据与前几帧亮度。
import sys

import cv2


no_camera = "--no-camera" in sys.argv[1:]
if no_camera:
    cap = cv2.VideoCapture("素材/synth_cube.mp4")
    source = "video"
else:
    cap = cv2.VideoCapture(0)
    source = "camera"
    if not cap.isOpened():
        cap.release()
        cap = cv2.VideoCapture("素材/synth_cube.mp4")
        source = "video"

print("source:", source)
print("opened:", cap.isOpened())
print("width:", int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)))
print("height:", int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
print("fps:", cap.get(cv2.CAP_PROP_FPS))

means = []
for _ in range(3):
    ok, frame = cap.read()
    if not ok:
        break
    means.append(round(float(frame.mean()), 2))
cap.release()
print("means:", means)
