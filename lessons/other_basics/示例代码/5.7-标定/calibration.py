# 由内参与畸变系数投影三维点，并计算与观测点的重投影误差。
import math

fx, fy = 800.0, 800.0
cx, cy = 640.0, 512.0
k1, k2 = -0.12, 0.05

points = [
    (0.0, 0.0, 2000.0),
    (100.0, -50.0, 2000.0),
    (-150.0, 80.0, 2500.0),
]
observed = [
    (640.4, 511.7),
    (680.3, 491.8),
    (591.7, 537.9),
]


def project(x, y, z):
    xn = x / z
    yn = y / z
    r2 = xn * xn + yn * yn
    radial = 1.0 + k1 * r2 + k2 * r2 * r2
    xd = xn * radial
    yd = yn * radial
    return fx * xd + cx, fy * yd + cy


print(f"内参：fx = {fx:.3f}, fy = {fy:.3f}, cx = {cx:.3f}, cy = {cy:.3f}")
print(f"畸变：k1 = {k1:.3f}, k2 = {k2:.3f}")

total = 0.0
for i, ((x, y, z), (ou, ov)) in enumerate(zip(points, observed), 1):
    u, v = project(x, y, z)
    err = math.hypot(u - ou, v - ov)
    total += err * err
    print(f"点 {i}：投影 ({u:.3f}, {v:.3f})，观测 ({ou:.3f}, {ov:.3f})，"
          f"误差 {err:.3f} 像素")

rms = math.sqrt(total / len(points))
print(f"重投影误差 RMS = {rms:.3f} 像素")
