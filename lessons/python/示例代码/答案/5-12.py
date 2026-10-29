# 练习 5-12：用三张棋盘图标定，打印焦距与重投影误差，再核对去畸变。
import cv2
import numpy as np


pattern = (9, 6)
object_points = np.zeros((pattern[0] * pattern[1], 3), np.float32)
object_points[:, :2] = np.mgrid[0:pattern[0], 0:pattern[1]].T.reshape(-1, 2)
object_points *= 40.0

objects = []
images = []
first_image = None
for index in range(1, 4):
    image = cv2.imread("素材/synth_chess_%d.png" % index)
    if first_image is None:
        first_image = image
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    ok, corners = cv2.findChessboardCorners(
        gray, pattern, flags=cv2.CALIB_CB_ADAPTIVE_THRESH |
        cv2.CALIB_CB_NORMALIZE_IMAGE)
    corners = cv2.cornerSubPix(
        gray, corners, (11, 11), (-1, -1),
        (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001))
    objects.append(object_points)
    images.append(corners)

rms, camera, dist, _, _ = cv2.calibrateCamera(
    objects, images, first_image.shape[1::-1], None, None)
print("rms:", round(float(rms), 2))
print("fx/fy:", round(float(camera[0, 0]), 1),
      round(float(camera[1, 1]), 1))
print("dist:", np.round(dist.ravel()[:2], 2).tolist())

undistorted = cv2.undistort(first_image, camera, dist)
diff = np.abs(first_image.astype(np.int16) -
              undistorted.astype(np.int16)).mean()
print("diff:", round(float(diff), 2))
