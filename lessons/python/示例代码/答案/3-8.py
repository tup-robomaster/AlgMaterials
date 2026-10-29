# 练习 3-8：读 JSON 与 YAML 两份配置，逐层合并，打印结果。
import json
from pathlib import Path

import yaml

base = json.loads(Path("素材/synth_config.json").read_text(encoding="utf-8"))
override = yaml.safe_load(Path("素材/synth_config.yaml").read_text(encoding="utf-8"))

merged = {}
for source in (base, override):
    for key, value in source.items():
        if isinstance(value, dict):
            merged.setdefault(key, {}).update(value)
        else:
            merged[key] = value

print("camera:", merged["camera"])
print("threshold:", merged["detector"]["threshold"])
print("verbose:", merged.get("verbose"))
print("keys:", sorted(merged))
print("json:", json.dumps(merged["camera"], sort_keys=True))
