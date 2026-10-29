# 给函数加类型标注与文档字符串，并观察运行时保存下来的标注。
from collections.abc import Iterable
from pprint import pprint


def mean(values: Iterable[float]) -> float:
    """返回一组数的平均值。"""
    items = list(values)
    return sum(items) / len(items)


def summarize(values: Iterable[float], digits: int = 2) -> tuple[int, float, float]:
    """返回数量、平均值与最大值。"""
    items = list(values)
    return len(items), round(mean(items), digits), max(items)


def label(score: float, name: str | None = None) -> str:
    """按分数给出等级；name 为空时只返回等级。"""
    level = "A" if score >= 90 else "B" if score >= 75 else "C"
    return level if name is None else f"{name}:{level}"


print(mean([88, 92, 79]))
print(summarize([88, 92, 79], digits=1))
print(label(92), label(60, "beta"))
print("mean annotations:")
pprint(mean.__annotations__, width=60, sort_dicts=False)
print("summarize annotations:")
pprint(summarize.__annotations__, width=60, sort_dicts=False)
print("label annotations:")
pprint(label.__annotations__, width=60, sort_dicts=False)
print("mean doc:", mean.__doc__)
