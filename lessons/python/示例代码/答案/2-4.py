# 练习 2-4：用 dataclass 表示包围盒，按面积筛选并观察相等比较。
from dataclasses import dataclass


@dataclass
class BoundingBox:
    x: int
    y: int
    width: int
    height: int

    @property
    def area(self):
        return self.width * self.height

    @property
    def center(self):
        return self.x + self.width / 2, self.y + self.height / 2


boxes = [
    BoundingBox(0, 0, 10, 20),
    BoundingBox(30, 10, 20, 20),
    BoundingBox(60, 40, 5, 5),
]
large = [box for box in boxes if box.area >= 200]
print("areas:", [box.area for box in boxes])
print("large:", [box.center for box in large])
ordered = sorted(boxes, key=lambda b: b.area, reverse=True)
print("sorted:", [box.area for box in ordered])
print("same:", BoundingBox(0, 0, 10, 20) == boxes[0])
