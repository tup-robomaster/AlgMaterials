# 练习 8-8：把预测时长放宽到 200 ms，比较两组系数的预测误差。
import csv
import subprocess
import sys
from pathlib import Path

result = subprocess.run(
    [sys.executable, "示例代码/8.4-replay.py", "--horizon-ms", "200",
     "--output-dir", "out_8_8"],
    capture_output=True, text=True)
if result.returncode != 0:
    raise SystemExit(result.stderr.strip())
with Path("out_8_8/out_metrics.csv").open(encoding="utf-8", newline="") as handle:
    rows = list(csv.DictReader(handle))
for row in rows:
    print("alpha %s: pred-error %s" % (row["alpha"], row["pred_error"]))
best = min(rows, key=lambda row: float(row["jitter"]) + float(row["pred_error"]))
print("best alpha:", best["alpha"])
