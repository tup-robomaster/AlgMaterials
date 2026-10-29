# 练习 3-7：用 DictReader 筛选高分行，再用 DictWriter 写出新表。
import csv
from pathlib import Path

rows = []
with Path("素材/synth_scores.csv").open(encoding="utf-8", newline="") as handle:
    for row in csv.DictReader(handle):
        try:
            score = int(row["score"])
        except (TypeError, ValueError):
            continue
        if score >= 80:
            rows.append({"name": row["name"], "score": score})

out = Path("out_high.csv")
with out.open("w", encoding="utf-8", newline="") as handle:
    writer = csv.DictWriter(handle, fieldnames=["name", "score"], lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

print("kept:", len(rows))
print("names:", [row["name"] for row in rows])
print("csv:", out.read_text(encoding="utf-8").strip().replace("\n", "; "))
