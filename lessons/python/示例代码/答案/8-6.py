# 练习 8-6：输入不存在时看退出码，并从日志末尾读失败原因。
import subprocess
import sys
from pathlib import Path

result = subprocess.run(
    [sys.executable, "示例代码/8.3-tool.py", "--input", "素材/missing.mp4",
     "--output-dir", "out_8_6"],
    capture_output=True, text=True)
lines = Path("out_8_6/out_run.log").read_text(encoding="utf-8").splitlines()
print("exit:", result.returncode)
print("last:", lines[-1])
