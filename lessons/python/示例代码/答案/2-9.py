# 练习 2-9：按固定大小切分序列，并观察生成器只能遍历一次。
def batches(items, size):
    if size <= 0:
        raise ValueError("size must be positive")
    for start in range(0, len(items), size):
        yield items[start:start + size]


data = list(range(1, 8))
for batch in batches(data, 3):
    print("batch:", batch)

stream = batches(["a", "b", "c", "d", "e", "f"], 2)
print("first:", next(stream))
print("rest:", list(stream))
print("exhausted:", list(stream))
