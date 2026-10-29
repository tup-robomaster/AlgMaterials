# 读入 素材/synth_notes.txt，按逗号拆开，再对齐打印成分数表。
#
# 运行：在课程根目录执行 python3 示例代码/1.2-strings.py
from pathlib import Path

lines = Path("素材/synth_notes.txt").read_text(encoding="utf-8").splitlines()
total = 0
rows = []
for line in lines:
    name, text = line.split(",")
    score = int(text)
    total += score
    rows.append((name, score))

for name, score in rows:
    level = "A" if score >= 85 else "B"
    print(f"{name:<8}{score:>4}  {level}")

mean = total / len(rows)
print(f"mean={mean:.2f}")
print("first line:", lines[0])
print("upper:", lines[0].upper())
print("slice:", lines[1][:4])
