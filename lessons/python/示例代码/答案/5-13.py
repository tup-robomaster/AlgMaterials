# 练习 5-13：同一阈值任务写两遍，计时后只打印稳定的判断结果。
import time

import numpy as np


rng = np.random.default_rng(3)
values = rng.integers(0, 256, size=(120, 160), dtype=np.uint8)
limit = 128


def loop_count(array):
    total = 0
    for row in array:
        for value in row:
            if value > limit:
                total += 1
    return total


def numpy_count(array):
    return int((array > limit).sum())


def elapsed(function, repeats=5):
    times = []
    for _ in range(repeats):
        start = time.perf_counter()
        function(values)
        times.append(time.perf_counter() - start)
    return sorted(times)[len(times) // 2]


loop_value = loop_count(values)
numpy_value = numpy_count(values)
loop_time = elapsed(loop_count)
numpy_time = elapsed(numpy_count)
print("count:", loop_value)
print("same:", loop_value == numpy_value)
print("faster:", "numpy" if numpy_time < loop_time else "loop")
print("timed runs:", 5)
