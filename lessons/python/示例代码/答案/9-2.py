# 练习 9-2：空目录与全坏目录的统计结果。
import tempfile
from pathlib import Path

import cv2

SUFFIXES = {".png", ".jpg", ".jpeg", ".bmp"}


def scan(folder):
    images = 0
    skipped = 0
    for path in sorted(Path(folder).iterdir()):
        if not path.is_file() or path.suffix.lower() not in SUFFIXES:
            continue
        if cv2.imread(str(path)) is None:
            skipped += 1
        else:
            images += 1
    return images, skipped


with tempfile.TemporaryDirectory() as empty:
    with tempfile.TemporaryDirectory() as bad:
        (Path(bad) / "broken.png").write_bytes(b"not a png\n")
        empty_images, empty_skipped = scan(empty)
        bad_images, bad_skipped = scan(bad)
print("empty images:", empty_images)
print("empty skipped:", empty_skipped)
print("bad images:", bad_images)
print("bad skipped:", bad_skipped)
