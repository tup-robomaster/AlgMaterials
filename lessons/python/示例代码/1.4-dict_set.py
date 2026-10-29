# 字典计数、取值与集合运算。
#
# 运行：在课程根目录执行 python3 示例代码/1.4-dict_set.py
from collections import Counter

text = "red green blue red green red"
counts = {}
for word in text.split():
    counts[word] = counts.get(word, 0) + 1
print("counts:", counts)

for word in sorted(counts):
    print(f"{word:<6}{counts[word]}")

freq = Counter(text.split())
print("most:", freq.most_common(2))

group_a = {"alpha", "beta", "gamma", "delta"}
group_b = {"gamma", "delta", "epsilon"}
print("common:", sorted(group_a & group_b))
print("only a:", sorted(group_a - group_b))
print("all:", sorted(group_a | group_b))

table = {"alpha": 92, "beta": 78}
print("get missing:", table.get("gamma", -1))
table.setdefault("gamma", 0)
table.update({"delta": 66})
print("keys:", sorted(table))
print("items:", sorted(table.items()))
