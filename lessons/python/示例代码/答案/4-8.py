# 练习 4-8：排序分数，返回排名，统计唯一值，并比较 nan 聚合。
import numpy as np


scores = np.array([72, 88, 91, 72, 65, 88, 100])
ordered = np.sort(scores)
print("sorted:", ordered.tolist())
print("top3:", ordered[-3:].tolist())
print("argsort:", np.argsort(scores, kind="stable").tolist())

unique, counts = np.unique(scores, return_counts=True)
print("unique:", unique.tolist(), counts.tolist())
print("median:", float(np.median(scores)))
print("p90:", round(float(np.percentile(scores, 90)), 1))

data = np.array([1.0, np.nan, 3.0, 5.0])
print("nanmean:", float(np.nanmean(data)))
print("nanmax:", float(np.nanmax(data)))
