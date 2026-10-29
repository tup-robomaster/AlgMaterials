# 批量图片统计：遍历目录，跳过坏图，写 CSV、JSON 与目标数量柱状图。
# 目标按饱和度与亮度筛出彩色区域，再按连通域面积计数。
import argparse
import csv
import json
import sys
from pathlib import Path

import cv2
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

SUFFIXES = {".png", ".jpg", ".jpeg", ".bmp"}


def build_parser():
    parser = argparse.ArgumentParser(description="统计目录中图片的尺寸与目标数量。")
    parser.add_argument("--input-dir", type=Path, default=Path("素材/批处理"),
                        help="图片目录")
    parser.add_argument("--output-dir", type=Path, default=Path("out_batch"),
                        help="输出目录")
    parser.add_argument("--min-area", type=float, default=200.0,
                        help="目标连通域的最小面积")
    return parser


def count_targets(image, min_area):
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    mask = ((hsv[:, :, 1] >= 60) & (hsv[:, :, 2] >= 60)).astype(np.uint8) * 255
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    return sum(1 for contour in contours
               if cv2.contourArea(contour) >= min_area)


def scan(input_dir, min_area):
    rows = []
    skipped = []
    for path in sorted(input_dir.iterdir()):
        if not path.is_file() or path.suffix.lower() not in SUFFIXES:
            continue
        image = cv2.imread(str(path))
        if image is None:
            skipped.append({"file": path.name, "reason": "cannot read image"})
            continue
        mean = image.mean(axis=(0, 1))
        rows.append({
            "file": path.name,
            "width": image.shape[1],
            "height": image.shape[0],
            "channels": image.shape[2],
            "mean_b": round(float(mean[0]), 2),
            "mean_g": round(float(mean[1]), 2),
            "mean_r": round(float(mean[2]), 2),
            "targets": count_targets(image, min_area),
        })
    return rows, skipped


def write_chart(path, rows):
    fig, ax = plt.subplots(figsize=(6, 3), dpi=100)
    names = [row["file"] for row in rows]
    counts = [row["targets"] for row in rows]
    if names:
        bars = ax.bar(names, counts, color="#4c78a8", width=0.55)
        for bar, count in zip(bars, counts):
            ax.text(bar.get_x() + bar.get_width() / 2, count + 0.2, str(count),
                    ha="center", va="bottom", fontsize=9)
        ax.set_ylim(0, max(counts) + 2)
    else:
        ax.text(0.5, 0.5, "no readable images", ha="center", va="center")
        ax.set_xticks([])
    ax.set_ylabel("targets")
    ax.set_title("Targets per image")
    ax.tick_params(axis="x", labelsize=8)
    ax.grid(True, axis="y", linewidth=0.4, alpha=0.5)
    fig.tight_layout()
    fig.savefig(path)
    plt.close(fig)


def main(argv=None):
    args = build_parser().parse_args(argv)
    if not args.input_dir.is_dir():
        print("error: not a directory: %s" % args.input_dir, file=sys.stderr)
        return 2
    try:
        args.output_dir.mkdir(parents=True, exist_ok=True)
    except OSError as error:
        print("error:", error, file=sys.stderr)
        return 2
    rows, skipped = scan(args.input_dir, args.min_area)
    csv_path = args.output_dir / "out_images.csv"
    json_path = args.output_dir / "out_report.json"
    plot_path = args.output_dir / "out_report.png"

    with csv_path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(["file", "width", "height", "channels",
                         "mean_b", "mean_g", "mean_r", "targets"])
        for row in rows:
            writer.writerow([row["file"], row["width"], row["height"],
                             row["channels"], "%.2f" % row["mean_b"],
                             "%.2f" % row["mean_g"], "%.2f" % row["mean_r"],
                             row["targets"]])

    report = {
        "images": len(rows),
        "input_dir": args.input_dir.as_posix(),
        "min_area": args.min_area,
        "skipped": skipped,
        "targets": sum(row["targets"] for row in rows),
    }
    json_path.write_text(
        json.dumps(report, ensure_ascii=False, indent=2,
                   sort_keys=True) + "\n",
        encoding="utf-8")
    write_chart(plot_path, rows)

    print("images:", len(rows))
    print("skipped:", len(skipped), end="")
    if skipped:
        print(" (%s)" % ", ".join(item["file"] for item in skipped))
    else:
        print()
    print("targets:", report["targets"])
    print("csv:", csv_path.as_posix())
    print("json:", json_path.as_posix())
    print("plot:", plot_path.as_posix())
    return 0


if __name__ == "__main__":
    sys.exit(main())
