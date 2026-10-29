# 用 argparse 读取输入、筛选与输出参数，main 返回退出码。
import argparse
import csv
from pathlib import Path


def build_parser():
    parser = argparse.ArgumentParser(description="统计分数表")
    parser.add_argument("--input", default="素材/synth_scores.csv")
    parser.add_argument("--top", type=int, default=2)
    parser.add_argument("--min-score", type=int, default=60)
    parser.add_argument("--output", default="out_stats.txt")
    return parser


def read_scores(path):
    scores = []
    with Path(path).open(encoding="utf-8", newline="") as handle:
        for row in csv.DictReader(handle):
            try:
                scores.append((row["name"], int(row["score"])))
            except (TypeError, ValueError):
                continue
    return scores


def main(argv=None):
    args = build_parser().parse_args(argv)
    scores = read_scores(args.input)
    passed = [(name, score) for name, score in scores if score >= args.min_score]
    top = sorted(passed, key=lambda item: item[1], reverse=True)[:args.top]
    lines = [f"input: {Path(args.input).name}", f"passed: {len(passed)}"]
    lines += [f"{name}:{score}" for name, score in top]
    text = "\n".join(lines)
    Path(args.output).write_text(text + "\n", encoding="utf-8")
    print(text)
    print("output:", Path(args.output).name)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
