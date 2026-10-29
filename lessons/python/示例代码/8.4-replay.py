# 离线回放：读入逐帧记录，比较多个平滑系数下的抖动、滞后与预测误差。
# 低置信度行保留时间位置，用上一次有效横坐标填充；结果写入 CSV 与曲线图。
import argparse
import csv
import statistics
import sys
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

CONFIDENCE = 0.5
MAX_LAG_STEPS = 10


def build_parser():
    parser = argparse.ArgumentParser(description="比较不同平滑系数的回放指标。")
    parser.add_argument("--input", type=Path, default=Path("素材/synth_track.csv"),
                        help="输入轨迹 CSV")
    parser.add_argument("--sweep", default="0.2,0.6",
                        help="逗号分隔的平滑系数，例如 0.2,0.6")
    parser.add_argument("--output-dir", type=Path, default=Path("."),
                        help="输出目录")
    parser.add_argument("--horizon-ms", type=int, default=100,
                        help="预测时长（毫秒）")
    return parser


def parse_sweep(text):
    values = []
    for part in text.split(","):
        part = part.strip()
        if not part:
            continue
        alpha = float(part)
        if not 0.0 < alpha <= 1.0:
            raise SystemExit("alpha out of range: %s" % part)
        values.append(alpha)
    if not values:
        raise SystemExit("empty sweep")
    return sorted(set(values))


def load_rows(path):
    with path.open(encoding="utf-8", newline="") as handle:
        rows = list(csv.DictReader(handle))
    if not rows:
        raise SystemExit("empty track file: %s" % path)
    times = [int(row["time_ms"]) for row in rows]
    raw = []
    low = 0
    last = None
    for row in rows:
        value = float(row["x"])
        if float(row["confidence"]) < CONFIDENCE and last is not None:
            low += 1
            raw.append(last)
        else:
            raw.append(value)
            last = value
    return times, raw, low


def smooth(values, alpha):
    level = values[0]
    result = []
    for value in values:
        level = level + alpha * (value - level)
        result.append(level)
    return result


def jitter(smoothed):
    steps = [abs(smoothed[i] - smoothed[i - 1])
             for i in range(1, len(smoothed))]
    return statistics.median(steps)


def lag_ms(times, raw, smoothed):
    step = (times[-1] - times[0]) / (len(times) - 1)
    best = None
    for delay in range(MAX_LAG_STEPS + 1):
        if delay >= len(smoothed):
            break
        error = sum(abs(smoothed[i] - raw[i - delay])
                    for i in range(delay, len(smoothed)))
        error /= len(smoothed) - delay
        if best is None or error < best[1]:
            best = (delay, error)
    return round(best[0] * step), best[1]


def prediction_error(times, raw, smoothed, horizon_ms):
    errors = []
    for i in range(1, len(smoothed) - 1):
        dt = times[i] - times[i - 1]
        if dt <= 0:
            continue
        target = times[i] + horizon_ms
        j = i + 1
        while j < len(times) - 1 and times[j] < target:
            j += 1
        rate = (smoothed[i] - smoothed[i - 1]) / dt
        predicted = smoothed[i] + rate * (times[j] - times[i])
        errors.append(abs(predicted - raw[j]))
    return sum(errors) / len(errors)


def main(argv=None):
    args = build_parser().parse_args(argv)
    alphas = parse_sweep(args.sweep)
    if args.horizon_ms <= 0:
        raise SystemExit("horizon must be positive")
    args.output_dir.mkdir(parents=True, exist_ok=True)
    times, raw, low = load_rows(args.input)
    metrics = []
    curves = {}
    for alpha in alphas:
        curve = smooth(raw, alpha)
        curves[alpha] = curve
        lag, _ = lag_ms(times, raw, curve)
        metrics.append({
            "alpha": alpha,
            "jitter": jitter(curve),
            "lag_ms": lag,
            "pred_error": prediction_error(times, raw, curve, args.horizon_ms),
        })
    best = min(metrics, key=lambda item: item["jitter"] + item["pred_error"])
    metrics_path = args.output_dir / "out_metrics.csv"
    plot_path = args.output_dir / "out_tracks.png"
    with metrics_path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(["alpha", "jitter", "lag_ms", "pred_error"])
        for item in metrics:
            writer.writerow(["%.2f" % item["alpha"], "%.3f" % item["jitter"],
                             item["lag_ms"], "%.3f" % item["pred_error"]])

    fig, ax = plt.subplots(figsize=(6, 3), dpi=100)
    seconds = [value / 1000.0 for value in times]
    ax.plot(seconds, raw, color="0.75", linewidth=1.0, label="raw")
    colors = ["#1f77b4", "#d62728", "#2ca02c", "#9467bd"]
    for alpha, color in zip(alphas, colors):
        ax.plot(seconds, curves[alpha], color=color, linewidth=1.6,
                label="alpha %.1f" % alpha)
    ax.set_xlabel("time (s)")
    ax.set_ylabel("x (px)")
    ax.set_title("Replay smoothing")
    ax.legend(loc="lower right", fontsize=8)
    ax.grid(True, linewidth=0.4, alpha=0.5)
    fig.tight_layout()
    fig.savefig(plot_path)
    plt.close(fig)

    print("samples:", len(times))
    print("low confidence:", low)
    for item in metrics:
        print("alpha %.2f: jitter %.3f lag %d ms pred-error %.3f"
              % (item["alpha"], item["jitter"], item["lag_ms"],
                 item["pred_error"]))
    print("best alpha: %.2f" % best["alpha"])
    print("metrics:", metrics_path.as_posix())
    print("plot:", plot_path.as_posix())
    return 0


if __name__ == "__main__":
    sys.exit(main())
