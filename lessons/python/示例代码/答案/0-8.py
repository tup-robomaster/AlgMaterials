# 练习 0-8：读取一张灰度 PNG，打印形状、像素类型与像素总数。
import cv2

image = cv2.imread("素材/synth_grid.png")
print("shape:", image.shape)
print("dtype:", image.dtype)
print("pixels:", image.shape[0] * image.shape[1])
