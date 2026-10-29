# 批量读取 素材 里的 PNG，打印文件名、宽高、通道数与像素类型，并写出结果文件。
#
# 运行：在课程根目录执行 python3 示例代码/0.4-image_sizes.py
from pathlib import Path

import cv2


def main():
    rows = []
    for path in sorted(Path("素材").glob("*.png")):
        image = cv2.imread(str(path))
        if image is None:
            print("读入失败:", path.name)
            continue
        height, width = image.shape[:2]
        channels = image.shape[2] if image.ndim == 3 else 1
        rows.append((path.name, width, height, channels, image.dtype))

    for name, width, height, channels, dtype in rows:
        print(f"{name:<18}{width:>4}x{height:<4} channels={channels}  {dtype}")

    lines = [f"{name},{width},{height},{channels},{dtype}"
             for name, width, height, channels, dtype in rows]
    Path("out_sizes.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"{len(rows)} images -> out_sizes.txt")


if __name__ == "__main__":
    main()
