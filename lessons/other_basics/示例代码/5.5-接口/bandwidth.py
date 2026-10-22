# 计算单帧数据量与接口可支持的最高帧率。
w, h = 1280, 1024

for bits in (8, 10, 12):
    frame_mb = w * h * bits / 8.0 / 1e6
    print(f"{w} x {h} Mono{bits}（按 {bits} bit 打包）：{frame_mb:.3f} MB/帧")

frame_mb = w * h * 8 / 8.0 / 1e6
for name, mbps in (("GigE", 125.0), ("USB 3.0", 625.0), ("10GigE", 1250.0)):
    fps = mbps / frame_mb
    print(f"{name}：Mono8 最多 {fps:.3f} fps（理论带宽，未扣协议开销）")

need = frame_mb * 100
print(f"Mono8、100 fps 需要 {need:.3f} MB/s")
print("GigE 只有 125.000 MB/s：带宽不足，会丢帧")
