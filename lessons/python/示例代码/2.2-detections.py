# 用 dataclass 保存检测结果，按面积排序，并用组合组织一帧里的目标。
from dataclasses import dataclass, field


@dataclass
class Detection:
    label: str
    x: int
    y: int
    width: int
    height: int
    score: float = 0.0
    tags: list[str] = field(default_factory=list)

    @property
    def area(self):
        return self.width * self.height

    @property
    def center(self):
        return self.x + self.width / 2, self.y + self.height / 2

    def __repr__(self):
        return (f"Detection({self.label!r}, area={self.area}, "
                f"score={self.score:.2f})")


class Scene:
    def __init__(self, name):
        self.name = name
        self.items = []

    def add(self, detection):
        self.items.append(detection)

    def large(self, minimum):
        return [item for item in self.items if item.area >= minimum]


items = [
    Detection("ball", 10, 20, 30, 40, 0.91),
    Detection("plate", 100, 50, 80, 60, 0.75),
    Detection("ball", 200, 10, 20, 20, 0.66),
]
scene = Scene("frame-01")
for item in items:
    scene.add(item)

for item in sorted(scene.items, key=lambda det: det.area, reverse=True):
    print(item)
print("count:", len(scene.items))
print("large:", len(scene.large(1000)))
print("centers:", [item.center for item in sorted(items, key=lambda det: det.label)])
print("tags independent:", items[0].tags is not items[1].tags)
