# 练习 3-9：用参数列表调用 argparse，并捕获 choice 不合法时的退出码。
import argparse
import contextlib
import io


def build_parser():
    parser = argparse.ArgumentParser(prog="tool")
    parser.add_argument("input")
    parser.add_argument("--top", type=int, default=3)
    parser.add_argument("--mode", choices=["fast", "full"], default="fast")
    return parser


args = build_parser().parse_args(["data.csv", "--top", "5", "--mode", "full"])
print("input:", args.input)
print("top:", args.top)
print("mode:", args.mode)

buffer = io.StringIO()
with contextlib.redirect_stderr(buffer):
    try:
        build_parser().parse_args(["data.csv", "--mode", "slow"])
    except SystemExit as error:
        print("exit:", error.code)
print("error:", "invalid choice" in buffer.getvalue())
