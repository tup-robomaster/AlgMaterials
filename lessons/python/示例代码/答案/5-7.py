# 练习 5-7：逐帧读取视频，统计帧数并抽样计算平均亮度。
import cv2


cap = cv2.VideoCapture("素材/synth_cube.mp4")
print("opened:", cap.isOpened())
print("size:", int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)),
      int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
print("fps:", cap.get(cv2.CAP_PROP_FPS))

frames = 0
means = []
while True:
    ok, frame = cap.read()
    if not ok:
        break
    if frames % 6 == 0:
        means.append(round(float(frame.mean()), 2))
    frames += 1
cap.release()

print("frames:", frames)
print("samples:", len(means))
print("mean of samples:", round(float(sum(means) / len(means)), 2))
print("first/last:", means[0], means[-1])
