# 由分辨率和像元尺寸计算靶面尺寸与单帧数据量。
import math


def sensor_info(name, w_px, h_px, pitch_um, bits):
    w_mm = w_px * pitch_um / 1000.0
    h_mm = h_px * pitch_um / 1000.0
    diag_mm = math.hypot(w_mm, h_mm)
    frame_mb = w_px * h_px * bits / 8.0 / 1e6
    print(f"{name}：{w_px} x {h_px}")
    print(f"  像元 {pitch_um:.2f} um，靶面 {w_mm:.3f} x {h_mm:.3f} mm")
    print(f"  对角线 {diag_mm:.3f} mm，{bits} bit 每帧 {frame_mb:.3f} MB")


sensor_info("相机 A", 1280, 1024, 10.0, 8)
sensor_info("相机 B", 2448, 2048, 3.45, 8)
