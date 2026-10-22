# 由拍摄范围选焦距，并估算景深。
sensor_w = 12.8
resolution = 1280
working_distance = 500.0
required_fov = 300.0
coc = 0.0106
aperture = 4.0

standard = [4.0, 6.0, 8.0, 12.0, 16.0, 25.0, 35.0, 50.0]
needed = sensor_w * working_distance / required_fov
chosen = max(f for f in standard if f <= needed)
actual_fov = sensor_w * working_distance / chosen
pixel_mm = actual_fov / resolution

print(f"需要焦距 = {needed:.3f} mm，选 {chosen:.0f} mm")
print(f"实际水平视场 = {actual_fov:.3f} mm（{working_distance:.0f} mm 处）")
print(f"放大倍率 = {sensor_w / actual_fov:.4f}")
print(f"每像素约 {pixel_mm:.4f} mm")

h = chosen * chosen / (aperture * coc) + chosen
s = working_distance
near = h * s / (h + (s - chosen))
far = h * s / (h - (s - chosen))
print(f"F{aperture:.1f}、弥散圆 {coc:.4f} mm 时：")
print(f"景深近界 = {near:.3f} mm，远界 = {far:.3f} mm")
print(f"景深 = {far - near:.3f} mm")
