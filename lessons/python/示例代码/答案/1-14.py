# 练习 1-14：观察导入、类与实例的命名空间，以及函数局部名字。
import math
from math import sqrt

scale = 2


class Point:
    dimensions = 2

    def __init__(self, x, y):
        self.x = x
        self.y = y


def local_names(value):
    doubled = value * 2
    return sorted(locals())


point = Point(6, 8)
print("sqrt:", sqrt(25))
print("hypot:", round(math.hypot(point.x, point.y), 1))
print("class keys:", sorted(name for name in vars(Point) if not name.startswith("__")))
print("instance keys:", sorted(vars(point)))
print("locals:", local_names(scale))
print("imported:", "math" in globals(), "sqrt" in globals())
