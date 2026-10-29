# 练习 3-10：用 --min-score 与 --output 控制筛选和输出文件。
import argparse
from pathlib import Path


def build_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("--min-score", type=int, default=60)
    parser.add_argument("--output", default="out/min.txt")
    return parser


args = build_parser().parse_args(["--min-score", "80", "--output", "out/pass.txt"])
scores = [92, 78, 85, 66]
passed = [score for score in scores if score >= args.min_score]
path = Path(args.output)
path.parent.mkdir(parents=True, exist_ok=True)
path.write_text("\n".join(str(score) for score in passed) + "\n", encoding="utf-8")
print("min:", args.min_score)
print("passed:", passed)
print("file:", path.name)
print("content:", path.read_text(encoding="utf-8").split())
