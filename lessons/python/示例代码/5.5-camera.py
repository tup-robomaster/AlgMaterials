# 优先打开摄像头；没有设备或带 --no-camera 时回退到课程视频。
import sys

import cv2


def open_source(no_camera):
    if not no_camera:
        cap = cv2.VideoCapture(0)
        if cap.isOpened():
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
            cap.set(cv2.CAP_PROP_FPS, 30)
            return cap, "camera"
        cap.release()
    return cv2.VideoCapture("素材/synth_cube.mp4"), "video"


no_camera = "--no-camera" in sys.argv[1:]
cap, source = open_source(no_camera)
print("source:", source)
print("opened:", cap.isOpened())
print("size:", int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)),
      int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
print("fps:", cap.get(cv2.CAP_PROP_FPS))

frames = 0
total = 0.0
saved = False
while frames < 12:
    ok, frame = cap.read()
    if not ok:
        break
    if frames == 0:
        saved = bool(cv2.imwrite("out_fallback.png", frame))
    total += float(frame.mean())
    frames += 1
    if source == "camera":
        cv2.imshow("camera", frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

print("frames:", frames)
print("mean:", round(total / frames, 2) if frames else 0.0)
print("snapshot saved:", saved)
cap.release()
if source == "camera":
    cv2.destroyAllWindows()
