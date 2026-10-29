# 练习闭包与参数形式：计数器保存状态，partial 固定前面的参数。
from functools import partial


def make_counter(start=0, step=1):
    count = start

    def counter():
        nonlocal count
        count += step
        return count

    return counter


def describe(name, /, value, *, digits=2):
    return f"{name}={value:.{digits}f}"


def append_bad(value, bucket=[]):
    bucket.append(value)
    return bucket


counter_a = make_counter()
counter_b = make_counter(10, 5)
print("a:", counter_a(), counter_a())
print("b:", counter_b(), counter_b())
print("a again:", counter_a())
print(describe("gain", 2.34567))
print(describe("gain", 2.34567, digits=1))
print("bad1:", append_bad(1))
print("bad2:", append_bad(2))
scale = partial(describe, "scale")
print(scale(0.5, digits=3))
