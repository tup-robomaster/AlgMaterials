# 在包外建立 near 子命令，核对成功、空结果与输入错误的退出码。
import argparse
import contextlib
import io
import sys

sys.path.insert(0, "示例代码/7.1-project")
from depth_report.io import read_measurements
from depth_report.stats import near


def run_near(args):
    names = near(read_measurements(args.path), args.limit)
    print("near %.1f: %d" % (args.limit, len(names)))
    if names:
        print(", ".join(names))
    return 0 if names else 1


def build_parser():
    parser = argparse.ArgumentParser(prog="report")
    sub = parser.add_subparsers(dest="command", required=True)
    nearby = sub.add_parser("near")
    nearby.add_argument("path")
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


path = "示例代码/7.1-project/data/measurements.csv"
print("exit:", main(["near", path, "--limit", "12.0"]))
print("exit:", main(["near", path, "--limit", "9.0"]))
with contextlib.redirect_stderr(io.StringIO()):
    code = main(["near", "missing.csv"])
print("missing:", code)
