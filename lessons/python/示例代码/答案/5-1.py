# 练习 5-1：读取两张图，比较形状、灰度图与像素通道。
import cv2
import numpy as np


shapes = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_COLOR)
balls = cv2.imread("素材/synth_balls.png", cv2.IMREAD_COLOR)
print("shapes:", shapes.shape, shapes.dtype)
print("balls:", balls.shape, balls.dtype)
print("same size:", shapes.shape == balls.shape)

gray = cv2.cvtColor(balls, cv2.COLOR_BGR2GRAY)
print("gray:", gray.shape, gray.dtype)
print("pixel BGR/gray:", balls[100, 100].tolist(), int(gray[100, 100]))
print("means:", np.round(shapes.mean(axis=(0, 1)), 2).tolist())
print("ball means:", np.round(balls.mean(axis=(0, 1)), 2).tolist())
