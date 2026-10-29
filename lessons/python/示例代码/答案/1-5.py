# 练习 1-5：去重、从大到小排序，打印前三名与 12 的出现次数。
values = [5, 12, 7, 12, 3, 9, 12]
unique = sorted(set(values), reverse=True)
for rank, value in enumerate(unique[:3], start=1):
    print(rank, value)
print("count 12:", values.count(12))
