# 调用工程里的 near 函数，列出深度不超过 15 的样本。
import sys

sys.path.insert(0, "示例代码/7.1-project")
from depth_report.io import read_measurements
from depth_report.stats import near


rows = read_measurements("示例代码/7.1-project/data/measurements.csv")
names = near(rows, 15.0)
print("near 15.0:", ", ".join(names))
print("count:", len(names))
