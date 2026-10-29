# 练习 2-7：给三个函数补类型标注，并打印运行时保存的标注。
from pprint import pprint


def clamp(value: float, low: float = 0.0, high: float = 1.0) -> float:
    return max(low, min(value, high))


def scale(values: list[float], factor: float) -> list[float]:
    return [value * factor for value in values]


def first_name(record: dict[str, str]) -> str | None:
    return record.get("name")


print(clamp(1.4))
print(scale([1.0, 2.0], 2.5))
print(first_name({"name": "alpha"}), first_name({}))
print("clamp annotations:")
pprint(clamp.__annotations__, width=60, sort_dicts=False)
print("scale annotations:")
pprint(scale.__annotations__, width=60, sort_dicts=False)
print("first_name annotations:")
pprint(first_name.__annotations__, width=60, sort_dicts=False)
