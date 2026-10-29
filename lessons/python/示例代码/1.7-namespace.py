# 观察模块、函数、类与对象的命名空间，并用 SimpleNamespace 组织配置。
from types import SimpleNamespace

threshold = 10


def classify(value):
    threshold = 5
    return "small" if value < threshold else "large"


def local_names(value):
    doubled = value * 2
    return sorted(locals())


class Box:
    kind = "box"

    def __init__(self, name):
        self.name = name


config = SimpleNamespace(threshold=5, label="ball")

print("classify(3):", classify(3))
print("classify(7):", classify(7))
print("module threshold:", threshold)
print("locals:", local_names(3))
print("class kind:", Box.kind)
print("instance dict:", vars(Box("A")))
print("config:", config.threshold, config.label)
print("config keys:", sorted(vars(config)))

for name in sorted(name for name in globals() if not name.startswith("__")):
    print("name:", name)
