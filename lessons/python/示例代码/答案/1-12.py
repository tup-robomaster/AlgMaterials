# 练习 1-12：用 *args 与关键字参数返回摘要，并用 None 避免共享默认列表。
def describe(name, *values, digits=1):
    return name, len(values), round(sum(values) / len(values), digits)


def append_safe(value, bucket=None):
    if bucket is None:
        bucket = []
    bucket.append(value)
    return bucket


print(describe("a", 1, 2, 3))
print(describe("b", 10, 20, digits=0))
print(append_safe(1))
print(append_safe(2))
