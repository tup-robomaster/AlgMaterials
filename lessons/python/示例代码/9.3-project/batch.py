# 检测题 9-7 的模块：图片统计、分数表读取与单工作线程批处理。
# 页面以辅助文件展示；答案脚本从这里导入，并用 unittest 核对两个函数。
import csv
import queue
import threading
from pathlib import Path

import cv2
import numpy as np

SUFFIXES = {".png", ".jpg", ".jpeg", ".bmp"}


def image_stats(path, min_area=200.0):
    image = cv2.imread(str(path))
    if image is None:
        raise OSError("cannot read image: %s" % Path(path).name)
    mean = image.mean(axis=(0, 1))
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    mask = ((hsv[:, :, 1] >= 60) & (hsv[:, :, 2] >= 60))
    mask = mask.astype(np.uint8) * 255
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    targets = sum(1 for contour in contours
                  if cv2.contourArea(contour) >= min_area)
    return {
        "width": image.shape[1],
        "height": image.shape[0],
        "channels": image.shape[2],
        "mean": round(float(mean.mean()), 2),
        "targets": targets,
    }


def read_scores(path):
    valid = []
    bad = []
    with Path(path).open(encoding="utf-8", newline="") as handle:
        reader = csv.reader(handle)
        next(reader)
        for lineno, row in enumerate(reader, start=2):
            if len(row) != 3 or not row[2].strip().isdigit():
                bad.append(lineno)
                continue
            valid.append((row[0].strip(), int(row[2])))
    mean = sum(score for _, score in valid) / len(valid) if valid else 0.0
    return valid, bad, mean


def process_images(paths, min_area=200.0):
    jobs = queue.Queue()
    results = queue.Queue()
    for path in paths:
        jobs.put(path)

    def work():
        while True:
            try:
                path = jobs.get_nowait()
            except queue.Empty:
                return
            try:
                stats = image_stats(path, min_area)
                results.put({"file": Path(path).name, "stats": stats,
                             "error": None})
            except OSError as error:
                results.put({"file": Path(path).name, "stats": None,
                             "error": str(error)})

    thread = threading.Thread(target=work, name="image-worker")
    thread.start()
    thread.join()
    rows = []
    while True:
        try:
            rows.append(results.get_nowait())
        except queue.Empty:
            break
    return sorted(rows, key=lambda row: row["file"])
