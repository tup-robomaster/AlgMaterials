# 用 total 与 scale 两个子命令演示处理函数分派。
import argparse


def run_total(args):
    print("sum:", sum(args.values))


def run_scale(args):
    print("product:", args.value * args.factor)


parser = argparse.ArgumentParser(prog="calc")
sub = parser.add_subparsers(dest="command", required=True)
total = sub.add_parser("total")
total.add_argument("values", nargs="+", type=int)
total.set_defaults(handler=run_total)
scale = sub.add_parser("scale")
scale.add_argument("value", type=int)
scale.add_argument("--factor", type=int, default=2)
scale.set_defaults(handler=run_scale)

for argv in (["total", "3", "4", "5"], ["scale", "7", "--factor", "3"]):
    args = parser.parse_args(argv)
    args.handler(args)
