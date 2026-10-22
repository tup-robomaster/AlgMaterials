#!/usr/bin/env python3
# 读取 camera.json，计算视场角、单帧大小与数据量，写入 report.json。

import json
import math
import sys
from pathlib import Path


def load_camera(path):
    """读取并检查相机参数。"""
    try:
        data = json.loads(Path(path).read_text(encoding="utf-8"))
    except OSError as error:
        raise SystemExit(f"无法读取 {path}：{error.strerror}")
    except json.JSONDecodeError as error:
        raise SystemExit(
            f"{path} 不是有效的 JSON：第 {error.lineno} 行"
            f"第 {error.colno} 列"
        )

    required = [
        "name", "width", "height", "pixel_um", "focal_mm",
        "fx", "fy", "cx", "cy", "fps", "bits_per_pixel",
    ]
    missing = [key for key in required if key not in data]
    if missing:
        raise SystemExit("缺少字段：" + "、".join(missing))
    for key in required[1:]:
        value = data[key]
        if not isinstance(value, (int, float)) or value <= 0:
            raise SystemExit(f"字段 {key} 必须是正数")
    return data


def compute(camera):
    """由分辨率、像元、焦距与帧率计算四项结果。"""
    pixel_mm = camera["pixel_um"] / 1000
    sensor_width_mm = camera["width"] * pixel_mm
    sensor_height_mm = camera["height"] * pixel_mm
    horizontal_fov = 2 * math.atan(
        sensor_width_mm / (2 * camera["focal_mm"])
    )
    vertical_fov = 2 * math.atan(
        sensor_height_mm / (2 * camera["focal_mm"])
    )
    frame_bytes = (
        camera["width"] * camera["height"]
        * camera["bits_per_pixel"] // 8
    )
    bandwidth = frame_bytes * camera["fps"]
    return {
        "sensor_width_mm": round(sensor_width_mm, 4),
        "sensor_height_mm": round(sensor_height_mm, 4),
        "focal_px": round(camera["focal_mm"] / pixel_mm, 4),
        "horizontal_fov_deg": round(math.degrees(horizontal_fov), 6),
        "vertical_fov_deg": round(math.degrees(vertical_fov), 6),
        "frame_bytes": frame_bytes,
        "frame_mib": round(frame_bytes / (1024 * 1024), 4),
        "bandwidth_bytes_s": bandwidth,
        "bandwidth_mib_s": round(bandwidth / (1024 * 1024), 4),
    }


def main():
    path = Path(sys.argv[1] if len(sys.argv) > 1 else "camera.json")
    camera = load_camera(path)
    result = compute(camera)
    report = {**camera, **result}
    target = path.resolve().with_name("report.json")
    target.write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )

    print(f"相机：{camera['name']}")
    print(f"分辨率：{camera['width']} × {camera['height']}")
    print(f"像元：{camera['pixel_um']:.1f} µm")
    print(f"焦距：{camera['focal_mm']:.1f} mm")
    print(
        f"焦距折算：{result['focal_px']:.1f} px"
        f"（fx={camera['fx']:.1f}，fy={camera['fy']:.1f}）"
    )
    print(f"水平视场角：{result['horizontal_fov_deg']:.2f}°")
    print(f"垂直视场角：{result['vertical_fov_deg']:.2f}°")
    print(
        f"单帧：{result['frame_bytes']} 字节"
        f"（{result['frame_mib']:.2f} MiB）"
    )
    print(
        f"{camera['fps']} fps 数据量："
        f"{result['bandwidth_mib_s']:.2f} MB/s"
    )
    print(f"已写入 {target.name}")


if __name__ == "__main__":
    main()
