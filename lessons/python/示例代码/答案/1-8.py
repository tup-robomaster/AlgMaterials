# 练习 1-8：用 Counter 统计单词，并对两个集合求交、差、并。
from collections import Counter

text = "red blue red green blue red"
counts = Counter(text.split())
for word, count in sorted(counts.items(), key=lambda item: (-item[1], item[0])):
    print(word, count)

first = {"a", "b", "c"}
second = {"b", "c", "d"}
print("common:", sorted(first & second))
print("only first:", sorted(first - second))
print("all:", sorted(first | second))
