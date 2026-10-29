# 入口模块：读取工程内的数据文件，打印汇总与接近阈值的样本。
from pathlib import Path

from .io import read_measurements
from .stats import near, summarize


DATA = Path(__file__).resolve().parent.parent / "data" / "measurements.csv"


def main():
    rows = read_measurements(DATA)
    report = summarize(rows)
    print("rows:", report["count"])
    print("mean depth: %.2f" % report["mean_depth"])
    print("max area: %.1f" % report["max_area"])
    print("near:", ", ".join(near(rows, 12.0)))


if __name__ == "__main__":
    main()
