# 练习 1-10：按分数分档，查找第一个不及格分数，并用字典推导式加分。
scores = [95, 82, 71, 58, 88]

for score in scores:
    if score >= 90:
        level = "A"
    elif score >= 80:
        level = "B"
    elif score >= 70:
        level = "C"
    else:
        level = "D"
    print(score, level)

for score in scores:
    if score < 60:
        print("first fail:", score)
        break
else:
    print("no fail")

bonus = {score: score + 5 for score in scores if score >= 80}
print("bonus:", bonus)
