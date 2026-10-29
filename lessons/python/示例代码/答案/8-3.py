# 练习 8-3：把越线位置移到 x=100，复用计数模块再跑一次。
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "8.2-project"))
from target_count import process

summary = process(Path("素材/synth_cube.mp4"), Path("out_8_3"),
                  line_x=100.0, min_area=1000.0)
print("frames:", summary["frames"])
print("detected:", summary["detected"])
print("events:", summary["events"])
print("event frames:", summary["event_frames"])
