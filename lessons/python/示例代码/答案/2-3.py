# 练习 2-3：写一个带类属性、实例方法、__repr__ 与距离计算的 Point 类。
class Point:
    dimensions = 2

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance_to(self, other):
        return ((self.x - other.x) ** 2 + (self.y - other.y) ** 2) ** 0.5

    def __repr__(self):
        return f"Point({self.x}, {self.y})"


a = Point(0, 0)
b = Point(3, 4)
print(a, b)
print("distance:", a.distance_to(b))
print("dimensions:", Point.dimensions, a.dimensions)
b.x = 6
b.y = 8
print("distance:", a.distance_to(b))
print("attributes:", sorted(vars(b)))
