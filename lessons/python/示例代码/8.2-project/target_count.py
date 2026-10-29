# 视频目标计数的核心模块：检测橙色方块、按重心关联轨迹、统计越线次数。
# 8.2 的固定路径例子与 8.3 的命令行入口都从这里导入；页面只贴这一份实现。
from dataclasses import dataclass
from pathlib import Path
import csv
import json

import cv2
import numpy as np


LOWER = np.array([5, 100, 100])
UPPER = np.array([35, 255, 255])
KERNEL = np.ones((3, 3), np.uint8)


@dataclass
class Detection:
    frame: int
    x: float
    y: float
    area: float
    box: tuple


class Tracker:
    """按重心最近邻关联，并在目标从左向右穿过竖线时记一次事件。"""

    def __init__(self, line_x, max_step=40.0, debounce=5):
        self.line_x = line_x
        self.max_step = max_step
        self.debounce = debounce
        self.last = None
        self.events = []
        self.last_event = -debounce

    def update(self, detection):
        crossed = False
        if self.last is not None:
            dx = detection.x - self.last.x
            dy = detection.y - self.last.y
            if abs(dx) <= self.max_step and abs(dy) <= self.max_step:
                if (self.last.x < self.line_x <= detection.x
                        and detection.frame - self.last_event >= self.debounce):
                    self.events.append(detection.frame)
                    self.last_event = detection.frame
                    crossed = True
            else:
                self.last = None
        self.last = detection
        return crossed


def detect(frame, index, min_area):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, LOWER, UPPER)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    best = None
    for contour in contours:
        area = cv2.contourArea(contour)
        if area < min_area:
            continue
        if best is None or area > best[0]:
            best = (area, contour)
    if best is None:
        return None
    area, contour = best
    moments = cv2.moments(contour)
    if moments["m00"] == 0:
        return None
    x = moments["m10"] / moments["m00"]
    y = moments["m01"] / moments["m00"]
    box = cv2.boundingRect(contour)
    return Detection(index, x, y, area, box)


def annotate(frame, detection, tracker, line_x):
    x = int(round(line_x))
    cv2.line(frame, (x, 40), (x, 200), (255, 255, 255), 1)
    if detection is not None:
        bx, by, bw, bh = detection.box
        cv2.rectangle(frame, (bx, by), (bx + bw, by + bh), (0, 255, 0), 2)
        cx = int(round(detection.x))
        cy = int(round(detection.y))
        cv2.circle(frame, (cx, cy), 3, (0, 0, 255), -1)
    cv2.putText(frame, "events: %d" % len(tracker.events), (185, 25),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 1,
                cv2.LINE_AA)


def process(source, out_dir, line_x=180.0, min_area=1000.0, logger=None):
    source = Path(source)
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    if not source.is_file():
        raise OSError("cannot open video: %s" % source)
    cap = cv2.VideoCapture(str(source))
    if not cap.isOpened():
        raise OSError("cannot open video: %s" % source)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS)
    if width <= 0 or height <= 0 or fps <= 0:
        cap.release()
        raise OSError("bad video metadata: %s" % source)

    writer = cv2.VideoWriter(str(out_dir / "out_count.mp4"),
                             cv2.VideoWriter_fourcc(*"mp4v"), fps,
                             (width, height))
    if not writer.isOpened():
        cap.release()
        raise OSError("cannot open video writer")
    tracker = Tracker(line_x)
    records = []
    index = 0
    last_frame = None
    try:
        while True:
            ok, frame = cap.read()
            if not ok:
                break
            detection = detect(frame, index, min_area)
            if detection is None:
                if logger is not None:
                    logger.debug("frame %d: no target", index)
            else:
                tracker.update(detection)
                records.append(detection)
                if logger is not None:
                    logger.debug("frame %d x=%.1f area=%.1f",
                                 index, detection.x, detection.area)
            annotate(frame, detection, tracker, line_x)
            writer.write(frame)
            last_frame = frame
            if logger is not None and index % 12 == 0:
                logger.info("frame %d detected=%d events=%d",
                            index, len(records), len(tracker.events))
            index += 1
    finally:
        cap.release()
        writer.release()

    if index == 0 or last_frame is None:
        raise OSError("no frames read: %s" % source)
    if records:
        first = [round(records[0].x, 1), round(records[0].y, 1)]
        last = [round(records[-1].x, 1), round(records[-1].y, 1)]
    else:
        first = None
        last = None
    summary = {
        "frames": index,
        "detected": len(records),
        "events": len(tracker.events),
        "event_frames": tracker.events,
        "line_x": line_x,
        "min_area": min_area,
        "first": first,
        "last": last,
    }
    with (out_dir / "out_tracks.csv").open("w", encoding="utf-8",
                                           newline="") as handle:
        rows = csv.writer(handle)
        rows.writerow(["frame", "x", "y", "area"])
        for item in records:
            rows.writerow([item.frame, "%.2f" % item.x, "%.2f" % item.y,
                           "%.1f" % item.area])
    (out_dir / "out_summary.json").write_text(
        json.dumps(summary, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
        encoding="utf-8")
    cv2.imwrite(str(out_dir / "out_count_frame.png"), last_frame)
    return summary
