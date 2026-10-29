# 练习 1-6：用 zip 配对、按分数排序，并切出隔一个取的分数。
names = ["a", "b", "c", "d"]
scores = [90, 75, 88, 60]
pairs = list(zip(names, scores))
ranked = sorted(pairs, key=lambda pair: pair[1], reverse=True)
for rank, (name, score) in enumerate(ranked[:3], start=1):
    print(rank, f"{name}:{score}")
print("first two:", ranked[:2])
print("every other:", scores[::2])
