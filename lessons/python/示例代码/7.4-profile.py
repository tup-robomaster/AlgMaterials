# 用 cProfile 查看函数调用次数，用 timeit 比较循环与公式，并核对结果。
import cProfile
import pstats
import timeit


def sum_squares_loop(n):
    total = 0
    for value in range(n):
        total += value * value
    return total


def sum_squares_formula(n):
    return (n - 1) * n * (2 * n - 1) // 6


def profile_loop(n):
    profiler = cProfile.Profile()
    profiler.enable()
    result = sum_squares_loop(n)
    profiler.disable()
    stats = pstats.Stats(profiler)
    calls = 0
    primitives = 0
    for func, values in stats.stats.items():
        if func[2] == "sum_squares_loop":
            primitives, calls = values[0], values[1]
    return result, primitives, calls


result, primitives, calls = profile_loop(1000)
assert result == sum_squares_formula(1000)
number = 2000
loop_time = timeit.timeit("sum_squares_loop(1000)", globals=globals(),
                          number=number)
formula_time = timeit.timeit("sum_squares_formula(1000)", globals=globals(),
                             number=number)
print("result:", result)
print("primitive calls:", primitives)
print("calls:", calls)
print("repeats:", number)
print("faster:", "formula" if formula_time < loop_time else "loop")
