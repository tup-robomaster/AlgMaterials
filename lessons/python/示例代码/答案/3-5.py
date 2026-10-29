# 练习 3-5：用 pathlib 找出所有 PNG，打印文件名、大小与后缀集合。
from pathlib import Path

root = Path("素材")
pngs = sorted(path for path in root.iterdir() if path.suffix == ".png")
print("count:", len(pngs))
for path in pngs:
    print(path.stem, path.stat().st_size)
print("suffixes:", sorted({path.suffix for path in root.iterdir() if path.is_file()}))
