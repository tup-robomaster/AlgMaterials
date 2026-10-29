# 读 CSV 与 YAML 配置，校验字段与类型，写出 JSON 摘要。
import csv
import json
from pathlib import Path

import yaml

DEFAULTS = {
    "camera": {"width": 640, "height": 480, "fps": 30},
    "detector": {"threshold": 0.5, "min_area": 100},
    "verbose": False,
}


def load_config(path):
    data = yaml.safe_load(Path(path).read_text(encoding="utf-8"))
    config = {key: dict(value) if isinstance(value, dict) else value
              for key, value in DEFAULTS.items()}
    for key, value in data.items():
        if isinstance(value, dict):
            config.setdefault(key, {}).update(value)
        else:
            config[key] = value
    width = config["camera"]["width"]
    height = config["camera"]["height"]
    if not isinstance(width, int) or not isinstance(height, int):
        raise ValueError("camera width and height must be integers")
    if not 0.0 <= config["detector"]["threshold"] <= 1.0:
        raise ValueError("threshold must be in 0..1")
    return config


rows = []
with Path("素材/synth_scores.csv").open(encoding="utf-8", newline="") as handle:
    for row in csv.DictReader(handle):
        if row.get(None) or not row.get("name") or not row.get("score"):
            continue
        try:
            score = int(row["score"])
        except ValueError:
            continue
        rows.append((row["name"], score))

config = load_config("素材/synth_config.yaml")
summary = {
    "config": config,
    "rows": len(rows),
    "mean": round(sum(score for _, score in rows) / len(rows), 2),
}
Path("out_summary.json").write_text(
    json.dumps(summary, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
    encoding="utf-8")
print(json.dumps(summary, ensure_ascii=False, indent=2, sort_keys=True))
print("saved:", Path("out_summary.json").name)
