# 逐行读分数表，跳过空行与坏行，把汇总写到 out_summary.txt。
from pathlib import Path

source = Path("素材/synth_scores.csv")
rows = []
skipped = 0
with source.open(encoding="utf-8") as handle:
    next(handle)
    for line in handle:
        text = line.strip()
        if not text:
            skipped += 1
            continue
        parts = text.split(",")
        if len(parts) != 3:
            skipped += 1
            continue
        name, subject, score_text = parts
        try:
            score = int(score_text)
        except ValueError:
            skipped += 1
            continue
        rows.append((name, subject, score))

total = sum(score for _, _, score in rows)
summary = [
    f"rows: {len(rows)}",
    f"skipped: {skipped}",
    f"total: {total}",
    f"mean: {total / len(rows):.2f}",
]
text = "\n".join(summary)
Path("out_summary.txt").write_text(text + "\n", encoding="utf-8")
print(text)
print("wrote:", Path("out_summary.txt").name)
