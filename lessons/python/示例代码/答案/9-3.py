# 练习 9-3：遍历目录，按后缀统计文件数与字节数，并处理空目录与缺失目录。
import csv
import tempfile
from pathlib import Path


def summarize(folder):
    if not folder.is_dir():
        return None
    counts = {}
    sizes = {}
    for path in sorted(folder.iterdir()):
        if not path.is_file():
            continue
        suffix = path.suffix.lower()
        counts[suffix] = counts.get(suffix, 0) + 1
        sizes[suffix] = sizes.get(suffix, 0) + path.stat().st_size
    return counts, sizes


source = Path("素材/批处理")
counts, sizes = summarize(source)
out_path = Path("out_9_3.csv")
with out_path.open("w", encoding="utf-8", newline="") as handle:
    writer = csv.writer(handle)
    writer.writerow(["suffix", "files", "bytes"])
    for suffix in sorted(counts):
        writer.writerow([suffix, counts[suffix], sizes[suffix]])
with tempfile.TemporaryDirectory() as empty:
    empty_counts, _ = summarize(Path(empty))
print("files:", sum(counts.values()))
print("png:", counts[".png"])
print("bytes:", sum(sizes.values()))
print("empty dir files:", sum(empty_counts.values()))
missing = summarize(Path("素材/missing"))
print("missing dir:", "skipped" if missing is None else "?")
print("csv: out_9_3.csv")
