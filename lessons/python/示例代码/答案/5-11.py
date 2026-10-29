# 练习 5-11：逐张检测棋盘角点，打印角点数量与第一个角点坐标。
import cv2


pattern = (9, 6)
for index in range(1, 4):
    image = cv2.imread("素材/synth_chess_%d.png" % index)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    ok, corners = cv2.findChessboardCorners(
        gray, pattern, flags=cv2.CALIB_CB_ADAPTIVE_THRESH |
        cv2.CALIB_CB_NORMALIZE_IMAGE)
    if not ok:
        print("image %d: failed" % index)
        continue
    corners = cv2.cornerSubPix(
        gray, corners, (11, 11), (-1, -1),
        (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001))
    x, y = corners.reshape(-1, 2)[0]
    first = [round(float(x), 2), round(float(y), 2)]
    print("image %d:" % index, len(corners), first)
