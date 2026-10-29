# 定义函数、默认参数、关键字参数、多返回值与可变默认值。
#
# 运行：在课程根目录执行 python3 示例代码/1.6-functions.py
def mean(values):
    return sum(values) / len(values)


def normalize(values):
    low, high = min(values), max(values)
    return [(value - low) / (high - low) for value in values]


def summarize(values, digits=2):
    return len(values), round(mean(values), digits), max(values)


def clamp(value, low=0.0, high=1.0):
    return max(low, min(value, high))


scores = [88, 92, 79, 95, 84]
print("mean:", mean(scores))
print("normalized:", [round(value, 2) for value in normalize(scores)])
print("summary:", summarize(scores))
print("summary(0):", summarize(scores, digits=0))
print("clamp:", clamp(1.4), clamp(-0.2), clamp(0.5))


def append_bad(value, bucket=[]):
    bucket.append(value)
    return bucket


print("bad1:", append_bad(1))
print("bad2:", append_bad(2))
