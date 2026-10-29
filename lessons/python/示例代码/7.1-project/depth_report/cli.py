# 命令行入口：用子命令读取测量记录，打印汇总或按深度筛选的结果。
import argparse
import sys
from pathlib import Path

from .io import read_measurements
from .stats import near, summarize


DATA = Path(__file__).resolve().parent.parent / "data" / "measurements.csv"


def run_summary(args):
    report = summarize(read_measurements(args.data))
    print("%s: %d rows, mean depth %.2f, max area %.1f"
          % (args.data.name, report["count"], report["mean_depth"],
             report["max_area"]))
    return 0


def run_near(args):
    names = near(read_measurements(args.data), args.limit)
    print("near %.1f: %d" % (args.limit, len(names)))
    if names:
        print(", ".join(names))
    return 0 if names else 1


def build_parser():
    parser = argparse.ArgumentParser(prog="depth-report",
                                     description="inspect measurement records")
    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--data", type=Path, default=DATA,
                        help="CSV file with name, depth and area")
    sub = parser.add_subparsers(dest="command", required=True)

    summary = sub.add_parser("summary", parents=[common],
                             help="print count and depth/area statistics")
    summary.set_defaults(handler=run_summary)

    nearby = sub.add_parser("near", parents=[common],
                            help="list samples within a depth limit")
    nearby.add_argument("--limit", type=float, default=12.0)
    nearby.set_defaults(handler=run_near)
    return parser


def main(argv=None):
    args = build_parser().parse_args(argv)
    try:
        return args.handler(args)
    except (OSError, ValueError) as error:
        print("error:", error, file=sys.stderr)
        return 3


if __name__ == "__main__":
    raise SystemExit(main())
