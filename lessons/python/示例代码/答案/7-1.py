# 把工程目录加入模块搜索路径，调用包里的读取与汇总函数。
import sys

sys.path.insert(0, "示例代码/7.1-project")
from depth_report.io import read_measurements
from depth_report.stats import summarize


rows = read_measurements("示例代码/7.1-project/data/measurements.csv")
report = summarize(rows)
print("rows:", report["count"])
print("mean depth: %.2f" % report["mean_depth"])
print("max area: %.1f" % report["max_area"])
