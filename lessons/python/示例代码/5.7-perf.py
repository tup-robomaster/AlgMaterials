# 固定输入与重复次数，比较 Python 逐像素循环与 NumPy 向量化。
import time
import timeit

import cv2
import numpy as np


image = cv2.imread("素材/synth_shapes.png", cv2.IMREAD_GRAYSCALE)
small = cv2.resize(image, (160, 120), interpolation=cv2.INTER_NEAREST)
limit = 120


def count_loop(array):
    total = 0
    for row in array:
        for value in row:
            if value > limit:
                total += 1
    return total


def count_numpy(array):
    return int((array > limit).sum())


def median_time(function, repeats=5):
    times = []
    for _ in range(repeats):
        start = time.perf_counter()
        function(small)
        times.append(time.perf_counter() - start)
    return sorted(times)[len(times) // 2]


loop_count = count_loop(small)
numpy_count = count_numpy(small)
print("count:", loop_count)
print("same:", loop_count == numpy_count)
print("loop ms:", round(median_time(count_loop) * 1000.0, 1))
print("numpy ms:", round(median_time(count_numpy) * 1000.0, 2))
print("timeit loop ms:", round(min(timeit.repeat(
    lambda: count_loop(small), repeat=3, number=1)) * 1000.0, 1))
print("timeit numpy ms:", round(min(timeit.repeat(
    lambda: count_numpy(small), repeat=3, number=1)) * 1000.0, 2))

roi = small[20:80, 40:120]
copied = roi.copy()
print("view shares:", bool(np.shares_memory(roi, small)))
print("copy shares:", bool(np.shares_memory(copied, small)))
