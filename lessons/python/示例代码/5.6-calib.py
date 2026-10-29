# 在三张棋盘图上找角点、标定相机，再去畸变并求一幅图的外参。
import cv2
import numpy as np


pattern = (9, 6)
square = 40.0
object_points = np.zeros((pattern[0] * pattern[1], 3), np.float32)
object_points[:, :2] = np.mgrid[0:pattern[0], 0:pattern[1]].T.reshape(-1, 2)
object_points *= square

objects = []
images = []
for index in range(1, 4):
    path = "素材/synth_chess_%d.png" % index
    image = cv2.imread(path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    ok, corners = cv2.findChessboardCorners(
        gray, pattern, flags=cv2.CALIB_CB_ADAPTIVE_THRESH |
        cv2.CALIB_CB_NORMALIZE_IMAGE)
    print("find %d:" % index, ok)
    if not ok:
        raise SystemExit("角点没有找全")
    corners = cv2.cornerSubPix(
        gray, corners, (11, 11), (-1, -1),
        (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001))
    objects.append(object_points)
    images.append(corners)
    print("corners:", len(corners))

rms, camera, dist, rvecs, tvecs = cv2.calibrateCamera(
    objects, images, gray.shape[::-1], None, None)
print("rms:", round(float(rms), 2))
print("fx/fy:", round(float(camera[0, 0]), 2),
      round(float(camera[1, 1]), 2))
print("cx/cy:", round(float(camera[0, 2]), 2),
      round(float(camera[1, 2]), 2))
print("dist:", np.round(dist.ravel(), 2).tolist())

first = cv2.imread("素材/synth_chess_1.png")
undistorted = cv2.undistort(first, camera, dist)
print("undistort saved:", bool(cv2.imwrite("out_undistort.png", undistorted)))
print("undistort diff:", round(float(np.abs(
    first.astype(np.int16) - undistorted.astype(np.int16)).mean()), 2))

map_x, map_y = cv2.initUndistortRectifyMap(
    camera, dist, None, camera, (640, 480), cv2.CV_32FC1)
remapped = cv2.remap(first, map_x, map_y, cv2.INTER_LINEAR)
print("remap shape:", remapped.shape)

ok, rvec, tvec = cv2.solvePnP(object_points, images[0], camera, dist)
print("pnp:", bool(ok), "t:", np.round(tvec.ravel(), 1).tolist())
