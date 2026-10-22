# 等价曝光、运动模糊与光源频闪的基本计算。
import math

base_f = 4.0
base_t_us = 10000.0
base_gain_db = 0.0

print(f"基准：F{base_f:.1f}, {base_t_us:.0f} us, {base_gain_db:.0f} dB")
print(f"快门 {base_t_us / 2:.0f} us 的等价设置：")
print(f"  F{base_f / math.sqrt(2):.1f}, {base_t_us:.0f} us, "
      f"{base_gain_db:.0f} dB（光圈开一档）")
print(f"  F{base_f:.1f}, {base_t_us:.0f} us, "
      f"{base_gain_db + 6:.0f} dB（增益加一档）")

speed = 0.5
focal = 16.0
working_distance = 500.0
pitch_um = 10.0
t_us = 5000.0
image_speed = speed * focal / working_distance
blur_mm = image_speed * t_us / 1e6 * 1000.0
blur_px = blur_mm * 1000.0 / pitch_um
print(f"运动模糊：速度 {speed:.3f} m/s，曝光 {t_us:.0f} us -> {blur_px:.3f} 像素")

period_ms = 1000.0 / 100.0
for t in (10000.0, 7500.0):
    cycles = (t / 1000.0) / period_ms
    print(f"50 Hz 光源：曝光 {t:.0f} us = {cycles:.3f} 个波动周期")
