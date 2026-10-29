# 练习 1-13：观察局部变量遮蔽，并用 SimpleNamespace 组织配置。
from types import SimpleNamespace

threshold = 0.5


def decide(score):
    threshold = 0.8
    return "pass" if score >= threshold else "fail"


print(decide(0.6))
print(decide(0.9))
print(threshold)

config = SimpleNamespace(threshold=0.5, label="ball")
print(config.threshold, config.label)
print(sorted(vars(config)))
