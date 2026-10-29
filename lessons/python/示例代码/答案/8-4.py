# 练习 8-4：核对逐帧记录的行数与首末重心，再读汇总里的事件数。
import csv
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "8.2-project"))
from target_count import process

out_dir = Path("out_8_4")
process(Path("素材/synth_cube.mp4"), out_dir, line_x=180.0,
        min_area=1000.0)
with (out_dir / "out_tracks.csv").open(encoding="utf-8", newline="") as handle:
    rows = list(csv.DictReader(handle))
summary = json.loads((out_dir / "out_summary.json").read_text(encoding="utf-8"))
print("rows:", len(rows))
print("first: %s,%s" % (rows[0]["x"], rows[0]["y"]))
print("last: %s,%s" % (rows[-1]["x"], rows[-1]["y"]))
print("events:", summary["events"])
