# 练习 5-8：隔帧读取，写出一个帧数减半的新视频。
from pathlib import Path

import cv2


cap = cv2.VideoCapture("素材/synth_cube.mp4")
out_dir = Path("out_video")
out_dir.mkdir(exist_ok=True)
writer = cv2.VideoWriter(str(out_dir / "every_other.mp4"),
                         cv2.VideoWriter_fourcc(*"mp4v"), 6.0, (320, 240))

frames = 0
written = 0
while True:
    ok, frame = cap.read()
    if not ok:
        break
    if frames % 2 == 0:
        cv2.putText(frame, "%03d" % frames, (250, 225),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1,
                    cv2.LINE_AA)
        writer.write(frame)
        written += 1
    frames += 1
cap.release()
writer.release()

print("read:", frames)
print("written:", written)
print("video saved:", (out_dir / "every_other.mp4").exists())
