# 针孔模型：把相机坐标系里的三维点投影到像素坐标。
import math

fx = 800.0
fy = 800.0
cx = 640.0
cy = 512.0
sensor_w = 12.8
sensor_h = 10.24
focal = 8.0

points = [
    (0.0, 0.0, 2000.0),
    (300.0, -200.0, 2000.0),
    (0.0, 0.0, 4000.0),
]

for x, y, z in points:
    u = fx * x / z + cx
    v = fy * y / z + cy
    print(f"({x:.0f}, {y:.0f}, {z:.0f}) mm -> 像素 ({u:.3f}, {v:.3f})")

fov_x = 2.0 * math.degrees(math.atan(sensor_w / (2.0 * focal)))
fov_y = 2.0 * math.degrees(math.atan(sensor_h / (2.0 * focal)))
print(f"水平视场角 = {fov_x:.3f}°")
print(f"垂直视场角 = {fov_y:.3f}°")
