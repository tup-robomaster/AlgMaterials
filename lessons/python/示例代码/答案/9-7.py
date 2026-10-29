# 练习 9-7：把图片统计与分数读取拆成模块，用单工作线程处理一批图片，
# 写 CSV 和柱状图，并用 unittest 核对两个函数。
import argparse
import csv
import io
import sys
import unittest
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

MODULE_DIR = Path(__file__).resolve().parent.parent / "9.3-project"
sys.path.insert(0, str(MODULE_DIR))
from batch import SUFFIXES, image_stats, read_scores, process_images


def build_parser():
    parser = argparse.ArgumentParser(description="批量统计图片并生成报告。")
    parser.add_argument("--input-dir", type=Path, default=Path("素材/批处理"),
                        help="图片目录")
    parser.add_argument("--output-dir", type=Path, default=Path("out_task5"),
                        help="输出目录")
    parser.add_argument("--min-area", type=float, default=200.0,
                        help="目标连通域的最小面积")
    return parser


def collect(folder):
    return sorted(path for path in folder.iterdir()
                  if path.is_file() and path.suffix.lower() in SUFFIXES)


def write_csv(path, rows):
    with path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(["file", "width", "height", "targets", "status"])
        for row in rows:
            stats = row["stats"]
            if stats is None:
                writer.writerow([row["file"], "", "", "", row["error"]])
            else:
                writer.writerow([row["file"], stats["width"], stats["height"],
                                 stats["targets"], "ok"])


def write_chart(path, rows):
    valid = [row for row in rows if row["stats"] is not None]
    fig, ax = plt.subplots(figsize=(6, 3), dpi=100)
    if valid:
        names = [row["file"] for row in valid]
        counts = [row["stats"]["targets"] for row in valid]
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


class BatchTest(unittest.TestCase):
    def test_image(self):
        stats = image_stats(Path("素材/synth_shapes.png"))
        self.assertEqual((stats["width"], stats["height"]), (640, 480))
        self.assertEqual(stats["targets"], 4)

    def test_scores(self):
        valid, bad, mean = read_scores(Path("素材/synth_scores.csv"))
        self.assertEqual(len(valid), 4)
        self.assertEqual(bad, [4, 6])
        self.assertAlmostEqual(mean, 80.25, places=2)


def main(argv=None):
    args = build_parser().parse_args(argv)
    if not args.input_dir.is_dir():
        print("error: not a directory: %s" % args.input_dir, file=sys.stderr)
        return 2
    args.output_dir.mkdir(parents=True, exist_ok=True)
    rows = process_images(collect(args.input_dir), args.min_area)
    write_csv(args.output_dir / "out_batch.csv", rows)
    write_chart(args.output_dir / "out_report.png", rows)
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(BatchTest)
    stream = io.StringIO()
    result = unittest.TextTestRunner(stream=stream, verbosity=0).run(suite)
    if not result.wasSuccessful():
        print(stream.getvalue())
        return 1
    valid = [row for row in rows if row["stats"] is not None]
    print("files:", len(rows))
    print("ok:", len(valid))
    print("failed:", len(rows) - len(valid))
    print("targets:", sum(row["stats"]["targets"] for row in valid))
    print("tests: OK")
    print("csv:", (args.output_dir / "out_batch.csv").as_posix())
    print("chart:", (args.output_dir / "out_report.png").as_posix())
    return 0


if __name__ == "__main__":
    sys.exit(main())
