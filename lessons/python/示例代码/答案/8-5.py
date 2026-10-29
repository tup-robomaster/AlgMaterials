# 练习 8-5：把最小面积提到 5000，目标全部被筛掉，再读报告。
import json
import subprocess
import sys
from pathlib import Path

result = subprocess.run(
    [sys.executable, "示例代码/8.3-tool.py", "--min-area", "5000",
     "--output-dir", "out_8_5"],
    capture_output=True, text=True)
if result.returncode != 0:
    raise SystemExit(result.stderr.strip())
report = json.loads(Path("out_8_5/out_report.json").read_text(encoding="utf-8"))
print("frames:", report["frames"])
print("detected:", report["detected"])
print("events:", report["events"])
