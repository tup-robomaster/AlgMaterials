# 生成课程用的合成素材。位置、颜色与尺寸都写死，重复运行得到同样的图。
# 第 6 部分四个例题也会各跑一遍，把输出的图写进素材目录。
#
# 运行：在课程根目录执行 python3 示例代码/gen_assets.py
# 也可以指定输出目录：python3 示例代码/gen_assets.py /tmp/素材
from pathlib import Path
import csv
import json
import math
import os
import shutil
import subprocess
import sys
import tempfile

import cv2
import numpy as np


def background(width, height):
    """从左到右由浅灰过渡到浅蓝的底色，避免纯色背景看不出边界。"""
    row = np.linspace(214, 238, width, dtype=np.float32)
    base = np.tile(row, (height, 1))
    image = np.dstack([base + 6, base + 2, base])
    return np.clip(image, 0, 255).astype(np.uint8)


def make_shapes(path):
    image = background(640, 480)
    cv2.rectangle(image, (80, 80), (260, 220), (190, 110, 40), -1)
    cv2.circle(image, (430, 150), 70, (40, 40, 200), -1)
    triangle = np.array([[170, 400], [320, 260], [470, 400]], np.int32)
    cv2.polylines(image, [triangle], True, (40, 160, 60), 4)
    cv2.line(image, (60, 440), (580, 440), (30, 180, 220), 3)
    cv2.imwrite(str(path), image)


def make_balls(path):
    image = background(640, 480)
    green = [(90 + 140 * col, 100 + 130 * row)
             for row in range(3) for col in range(4)]
    for x, y in green:
        cv2.circle(image, (x, y), 24, (50, 170, 60), -1)
    for x, y in [(520, 90), (560, 300)]:
        cv2.circle(image, (x, y), 24, (50, 50, 190), -1)
    cv2.imwrite(str(path), image)


def make_grid(path):
    cell = 20
    image = np.zeros((240, 320), np.uint8)
    for y in range(0, 240, cell):
        for x in range(0, 320, cell):
            if (x // cell + y // cell) % 2:
                image[y:y + cell, x:x + cell] = 210
            else:
                image[y:y + cell, x:x + cell] = 45
    cv2.imwrite(str(path), image)


def make_cube_video(path):
    """12 帧每秒、36 帧的短片：橙色的方块从左向右移动，左上角写帧号。"""
    width, height, count = 320, 240, 36
    writer = cv2.VideoWriter(str(path), cv2.VideoWriter_fourcc(*"mp4v"),
                             12.0, (width, height))
    if not writer.isOpened():
        raise RuntimeError("无法创建视频: %s" % path)
    try:
        for index in range(count):
            image = background(width, height)
            image = (image // 2).astype(np.uint8)
            x = 20 + index * 6
            cv2.rectangle(image, (x, 80), (x + 60, 160), (30, 160, 230), -1)
            cv2.rectangle(image, (x, 80), (x + 60, 160), (255, 255, 255), 2)
            cv2.putText(image, "%02d" % index, (10, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2,
                        cv2.LINE_AA)
            writer.write(image)
    finally:
        writer.release()


def chess_board(cols=9, rows=6, square=40, margin=24):
    """一块 10×7 格的黑白棋盘，外侧留白边；内角点数是 9×6。"""
    width = (cols + 1) * square + 2 * margin
    height = (rows + 1) * square + 2 * margin
    board = np.full((height, width, 3), 255, np.uint8)
    for row in range(rows + 1):
        for col in range(cols + 1):
            if (row + col) % 2 == 0:
                x = margin + col * square
                y = margin + row * square
                cv2.rectangle(board, (x, y), (x + square, y + square),
                              (20, 20, 20), -1)
    return board


def make_chess(path, rotation, distance):
    """把棋盘按给定的旋转角与距离投影到 640×480 的画面上。"""
    board = chess_board()
    height, width = board.shape[:2]
    camera = np.array([[520.0, 0.0, 320.0],
                       [0.0, 520.0, 240.0],
                       [0.0, 0.0, 1.0]])
    rot, _ = cv2.Rodrigues(np.asarray(rotation, np.float64))
    center = np.array([width / 2.0, height / 2.0, 0.0])
    tvec = np.array([0.0, 0.0, distance]) - rot @ center
    points = np.float32([[0, 0, 0], [width - 1, 0, 0],
                         [width - 1, height - 1, 0], [0, height - 1, 0]])
    projected, _ = cv2.projectPoints(points, np.asarray(rotation, np.float64),
                                     tvec, camera, np.zeros(5))
    corners = projected.reshape(-1, 2).astype(np.float32)
    source = np.float32([[0, 0], [width - 1, 0],
                         [width - 1, height - 1], [0, height - 1]])
    matrix = cv2.getPerspectiveTransform(source, corners)
    canvas = background(640, 480)
    image = cv2.warpPerspective(board, matrix, (640, 480), dst=canvas,
                                borderMode=cv2.BORDER_TRANSPARENT)
    cv2.imwrite(str(path), image)


def make_chessboards(out):
    make_chess(out / "synth_chess_1.png", (-0.5, 0.35, 0.1), 650.0)
    make_chess(out / "synth_chess_2.png", (0.55, 0.35, 0.3), 760.0)
    make_chess(out / "synth_chess_3.png", (0.0, 0.0, 0.0), 700.0)


def make_track(out):
    """离线回放用的逐帧记录：时间、横纵坐标与置信度，两行置信度偏低。"""
    target = out / "synth_track.csv"
    with target.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle, lineterminator="\n")
        writer.writerow(["time_ms", "x", "y", "confidence"])
        for index in range(60):
            t = index * 20
            x = (160 + 80 * math.sin(2 * math.pi * t / 600)
                 + 4 * math.sin(t * 0.37) + 2 * math.cos(t * 0.11))
            y = 120 + 20 * math.sin(2 * math.pi * t / 900) + 1.5 * math.cos(t * 0.23)
            confidence = 0.90 + 0.05 * math.sin(t / 200)
            if t in (300, 800):
                confidence = 0.40
            writer.writerow([t, "%.2f" % x, "%.2f" % y, "%.2f" % confidence])


def make_batch(out):
    """第 9 部分批量处理的输入：三张合成图与一张读不出来的坏图。"""
    target = out / "批处理"
    target.mkdir(parents=True, exist_ok=True)
    for name in ("synth_shapes.png", "synth_balls.png", "synth_grid.png"):
        shutil.copyfile(out / name, target / name)
    (target / "broken.png").write_bytes(b"not a png\n")


def make_examples(out):
    """跑 8.2、8.4、9.1，各取一张结果图放进素材目录下的综合示例。"""
    target = out / "综合示例"
    target.mkdir(parents=True, exist_ok=True)
    here = Path(__file__).resolve().parent
    env = dict(os.environ, OPENCV_LOG_LEVEL="ERROR")
    with tempfile.TemporaryDirectory(prefix="python-examples-") as tmp:
        work = Path(tmp)
        shutil.copytree(out, work / "素材")
        for name in ("8.2-count.py", "8.4-replay.py", "9.1-report.py"):
            subprocess.run([sys.executable, str(here / name)], cwd=str(work),
                           check=True, stdout=subprocess.DEVNULL, env=env)
        shutil.copyfile(work / "out_count_frame.png",
                        target / "out_count_frame.png")
        shutil.copyfile(work / "out_tracks.png", target / "out_tracks.png")
        shutil.copyfile(work / "out_batch" / "out_report.png",
                        target / "out_report.png")


PLOT_SCRIPTS = ("6.1-plot.py", "6.2-curves.py", "6.3-scatter.py",
                "6.4-saveplot.py")


def make_plots(out):
    """把第 6 部分四个例题各跑一遍，输出图写进素材目录下的图表示例。

    页面在例题上方展示这些图；图片与读者运行例题得到的文件同名、同内容，
    改例题后重新运行本脚本即可同步。放在子目录里，前面几课按素材根目录
    统计文件时不会多出这几个 PNG。
    """
    target = out / "图表示例"
    target.mkdir(parents=True, exist_ok=True)
    here = Path(__file__).resolve().parent
    for name in PLOT_SCRIPTS:
        subprocess.run([sys.executable, str(here / name)], cwd=str(target),
                       check=True, stdout=subprocess.DEVNULL)


def main():
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).resolve().parent.parent / "素材"
    out.mkdir(parents=True, exist_ok=True)
    make_shapes(out / "synth_shapes.png")
    make_balls(out / "synth_balls.png")
    make_grid(out / "synth_grid.png")
    make_chessboards(out)
    make_cube_video(out / "synth_cube.mp4")
    (out / "synth_notes.txt").write_text(
        "alpha,92\nbeta,78\ngamma,85\ndelta,66\n", encoding="utf-8")

    # 第 3 部分的输入：一张带空行与坏行的分数表，以及等价的 JSON、YAML 配置。
    (out / "synth_scores.csv").write_text(
        "name,subject,score\n"
        "alpha,math,92\n"
        "beta,math,78\n"
        "\n"
        "gamma,math,85\n"
        "delta,math,oops\n"
        "epsilon,math,66\n", encoding="utf-8")

    config = {
        "camera": {"fps": 30, "height": 480, "width": 640},
        "detector": {"min_area": 120, "threshold": 0.55},
        "verbose": False,
    }
    (out / "synth_config.json").write_text(
        json.dumps(config, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
        encoding="utf-8")
    (out / "synth_config.yaml").write_text(
        "camera:\n"
        "  fps: 30\n"
        "  height: 480\n"
        "  width: 640\n"
        "detector:\n"
        "  min_area: 120\n"
        "  threshold: 0.55\n", encoding="utf-8")
    make_track(out)
    make_batch(out)
    make_examples(out)
    make_plots(out)
    print("写入:", out)


if __name__ == "__main__":
    main()
