# 练习 8-7：只扫一个更高的平滑系数，读指标表并给出综合评分最低的一组。
import csv
import subprocess
import sys
from pathlib import Path

result = subprocess.run(
    [sys.executable, "示例代码/8.4-replay.py", "--sweep", "0.8",
     "--output-dir", "out_8_7"],
    capture_output=True, text=True)
if result.returncode != 0:
    raise SystemExit(result.stderr.strip())
with Path("out_8_7/out_metrics.csv").open(encoding="utf-8", newline="") as handle:
    rows = list(csv.DictReader(handle))
for row in rows:
    print("alpha %s: jitter %s lag %s ms pred-error %s"
          % (row["alpha"], row["jitter"], row["lag_ms"], row["pred_error"]))
best = min(rows, key=lambda row: float(row["jitter"]) + float(row["pred_error"]))
print("best alpha:", best["alpha"])
