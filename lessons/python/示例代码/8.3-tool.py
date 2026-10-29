# 命令行入口：参数解析、文件日志、报告 JSON 与可区分的错误码。
# 默认在 out_run/ 下写出视频、逐帧记录、汇总、报告与日志。
import argparse
import json
import logging
import sys
import time
from pathlib import Path

MODULE_DIR = Path(__file__).resolve().parent / "8.2-project"
sys.path.insert(0, str(MODULE_DIR))
from target_count import process


def build_parser():
    parser = argparse.ArgumentParser(
        description="统计视频中从左向右越过竖线的目标次数。")
    parser.add_argument("--input", type=Path, default=Path("素材/synth_cube.mp4"),
                        help="输入视频路径")
    parser.add_argument("--line", type=float, default=180.0, help="竖线的横坐标")
    parser.add_argument("--min-area", type=float, default=1000.0,
                        help="有效轮廓的最小面积")
    parser.add_argument("--output-dir", type=Path, default=Path("out_run"),
                        help="输出目录")
    parser.add_argument("--debug", action="store_true", help="日志中记录每一帧")
    return parser


def make_logger(path, debug):
    logger = logging.getLogger("target-count")
    logger.handlers.clear()
    logger.setLevel(logging.DEBUG)
    logger.propagate = False
    handler = logging.FileHandler(path, mode="w", encoding="utf-8")
    handler.setFormatter(logging.Formatter("%(levelname)s %(message)s"))
    handler.setLevel(logging.DEBUG if debug else logging.INFO)
    logger.addHandler(handler)
    return logger


def main(argv=None):
    args = build_parser().parse_args(argv)
    log_path = args.output_dir / "out_run.log"
    try:
        args.output_dir.mkdir(parents=True, exist_ok=True)
        logger = make_logger(log_path, args.debug)
    except OSError as error:
        print("error:", error, file=sys.stderr)
        return 4
    logger.info("start input=%s line=%.1f min_area=%.0f",
                args.input.name, args.line, args.min_area)
    start = time.perf_counter()
    try:
        summary = process(args.input, args.output_dir, line_x=args.line,
                          min_area=args.min_area, logger=logger)
    except OSError as error:
        logger.error("failed: %s", error)
        print("error:", error, file=sys.stderr)
        return 3
    elapsed_ms = round((time.perf_counter() - start) * 1000.0, 3)
    report = {
        "input": args.input.name,
        "frames": summary["frames"],
        "detected": summary["detected"],
        "events": summary["events"],
        "line_x": summary["line_x"],
        "min_area": summary["min_area"],
        "outputs": ["out_count.mp4", "out_tracks.csv", "out_summary.json",
                    "out_count_frame.png"],
        "elapsed_ms": elapsed_ms,
    }
    report_path = args.output_dir / "out_report.json"
    try:
        report_path.write_text(
            json.dumps(report, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
            encoding="utf-8")
    except OSError as error:
        logger.error("cannot write report: %s", error)
        print("error:", error, file=sys.stderr)
        return 4
    logger.info("done frames=%d events=%d elapsed_ms=%.3f",
                summary["frames"], summary["events"], elapsed_ms)
    print("input:", args.input.name)
    print("frames:", summary["frames"])
    print("events:", summary["events"])
    print("report:", report_path.as_posix())
    print("log:", log_path.as_posix())
    return 0


if __name__ == "__main__":
    sys.exit(main())
