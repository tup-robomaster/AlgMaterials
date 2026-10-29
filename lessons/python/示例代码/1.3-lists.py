# 列表的增删、排序、切片，以及 enumerate 与 zip 的配合。
#
# 运行：在课程根目录执行 python3 示例代码/1.3-lists.py
scores = [88, 92, 79, 95, 66, 84, 92]

print("raw:", scores)
print("len:", len(scores), "max:", max(scores), "min:", min(scores))

ordered = sorted(scores)
print("sorted:", ordered)
print("top3:", ordered[-3:])
print("middle:", scores[2:5])
print("every2:", scores[::2])

scores.append(100)
scores.remove(66)
print("changed:", scores)

pairs = list(enumerate(scores))
print("last pair:", pairs[-1])

names = ["a", "b", "c"]
for index, (name, score) in enumerate(zip(names, scores[:3])):
    print(index, name, score)
