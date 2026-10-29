# 用 cProfile 统计递归斐波那契的函数调用次数。
import cProfile
import pstats


def fib(n):
    if n < 2:
        return n
    return fib(n - 1) + fib(n - 2)


profiler = cProfile.Profile()
profiler.enable()
result = fib(10)
profiler.disable()
stats = pstats.Stats(profiler)
calls = 0
primitives = 0
for func, values in stats.stats.items():
    if func[2] == "fib":
        primitives, calls = values[0], values[1]
print("result:", result)
print("calls:", calls)
print("primitive:", primitives)
