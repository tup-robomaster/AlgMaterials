# 固定路径的计数例子：调用 8.2-project 里的模块，在合成视频上跑一次。
# 输出视频、逐帧记录、汇总 JSON 与最后一帧的标注图。
import sys
from pathlib import Path

MODULE_DIR = Path(__file__).resolve().parent / "8.2-project"
sys.path.insert(0, str(MODULE_DIR))
from target_count import process

summary = process(Path("素材/synth_cube.mp4"), Path("."),
                  line_x=180.0, min_area=1000.0)
print("frames:", summary["frames"])
print("detected frames:", summary["detected"])
print("events:", summary["events"])
print("event frames:", summary["event_frames"])
print("tracks: out_tracks.csv")
print("summary: out_summary.json")
print("video: out_count.mp4")
print("frame: out_count_frame.png")
