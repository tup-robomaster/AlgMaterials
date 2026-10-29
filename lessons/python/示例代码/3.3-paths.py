# 用 pathlib 遍历素材目录，按后缀分组并写一份清单。
from pathlib import Path

root = Path("素材")
files = sorted(path for path in root.iterdir() if path.is_file())
groups = {}
for path in files:
    groups.setdefault(path.suffix, []).append(path)

for suffix in sorted(groups):
    print(suffix, len(groups[suffix]))
for path in files:
    print(f"{path.name:<20}{path.stat().st_size:>7}")

out_dir = Path("out")
out_dir.mkdir(exist_ok=True)
manifest = out_dir / "manifest.txt"
lines = [f"{path.name},{path.stat().st_size}" for path in files]
manifest.write_text("\n".join(lines) + "\n", encoding="utf-8")
print("manifest:", manifest.name)
print("lines:", len(lines))
