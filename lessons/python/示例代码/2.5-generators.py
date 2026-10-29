# 生成器按批产出数据，装饰器记录调用次数并保留原函数的名字。
from functools import wraps


def batches(items, size):
    for start in range(0, len(items), size):
        yield items[start:start + size]


def counted(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        wrapper.calls += 1
        return func(*args, **kwargs)

    wrapper.calls = 0
    return wrapper


@counted
def total(values):
    return sum(values)


data = list(range(1, 11))
print("batches:", list(batches(data, 4)))
stream = batches(data, 3)
print("first:", next(stream))
print("second:", next(stream))
print("third:", next(stream))
print("fourth:", next(stream))
try:
    next(stream)
except StopIteration:
    print("stop")
print("total:", total([1, 2, 3]))
print("total:", total([4, 5]))
print("calls:", total.calls, total.__name__)
print("lazy:", sum(1 for value in data if value % 2 == 0))
