# 练习 3-1：读取纯文本，统计行数、空行、最长行与总字符数。
from pathlib import Path

path = Path("素材/synth_notes.txt")
lines = path.read_text(encoding="utf-8").splitlines()
print("lines:", len(lines))
print("empty:", sum(1 for line in lines if not line.strip()))
print("longest:", max(lines, key=len))
print("chars:", sum(len(line) for line in lines))
