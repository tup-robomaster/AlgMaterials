# 练习 0-7：统计宽度不小于 640 的图片，并打印它们的尺寸。
from pathlib import Path

import cv2

wide = []
for path in sorted(Path("素材").glob("*.png")):
    image = cv2.imread(str(path))
    if image is None:
        continue
    height, width = image.shape[:2]
    if width >= 640:
        wide.append((path.name, width, height))

print(f"{len(wide)} wide images")
for name, width, height in wide:
    print(f"{name} {width}x{height}")
