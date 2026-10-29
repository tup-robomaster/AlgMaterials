# 练习 9-4：跳过分数 CSV 的坏行，校验 JSON 与 YAML 配置并写出规范化结果。
import csv
import json
from pathlib import Path

import yaml


def read_scores(path):
    valid = []
    bad = []
    with path.open(encoding="utf-8", newline="") as handle:
        reader = csv.reader(handle)
        next(reader)
        for lineno, row in enumerate(reader, start=2):
            if len(row) != 3 or not row[2].strip().isdigit():
                bad.append(lineno)
                continue
            valid.append((row[0].strip(), int(row[2])))
    return valid, bad


def normalize(config):
    camera = config["camera"]
    detector = config["detector"]
    if not isinstance(camera, dict) or not isinstance(detector, dict):
        raise ValueError("camera and detector must be mappings")
    fps = camera["fps"]
    width = camera["width"]
    height = camera["height"]
    min_area = detector["min_area"]
    threshold = detector["threshold"]
    values = (fps, width, height, min_area)
    if not all(isinstance(value, int) for value in values):
        raise ValueError(
            "fps, width, height and min_area must be integers")
    if not isinstance(threshold, (int, float)):
        raise ValueError("threshold must be a number")
    verbose = config.get("verbose", False)
    if not isinstance(verbose, bool):
        raise ValueError("verbose must be a boolean")
    return {
        "camera": {"fps": fps, "width": width, "height": height},
        "detector": {
            "min_area": min_area,
            "threshold": float(threshold),
        },
        "verbose": verbose,
    }


valid, bad = read_scores(Path("素材/synth_scores.csv"))
json_config = json.loads(
    Path("素材/synth_config.json").read_text(encoding="utf-8"))
yaml_config = yaml.safe_load(
    Path("素材/synth_config.yaml").read_text(encoding="utf-8"))
json_ok = normalize(json_config)
yaml_ok = normalize(yaml_config)
mean = sum(score for _, score in valid) / len(valid)
Path("out_9_4.json").write_text(
    json.dumps(yaml_ok, ensure_ascii=False, indent=2,
               sort_keys=True) + "\n",
    encoding="utf-8")
print("valid rows:", len(valid))
print("bad lines:", ",".join(str(line) for line in bad))
print("mean score: %.2f" % mean)
print("json:", "ok" if json_ok else "bad")
print("yaml:", "ok" if yaml_ok else "bad")
print("normalized: out_9_4.json")
