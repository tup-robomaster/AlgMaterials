# 条件、循环与推导式：同一批分数用三种写法处理。
#
# 运行：在课程根目录执行 python3 示例代码/1.5-control_flow.py
scores = [88, 92, 79, 55, 95, 61]

for score in scores:
    if score >= 90:
        level = "A"
    elif score >= 75:
        level = "B"
    elif score >= 60:
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

passed = [score for score in scores if score >= 60]
print("passed:", passed)
print("bonus:", [score + 5 for score in passed])
squares = {score: score * score for score in passed[:3]}
print("squares:", squares)
print("max by:", max(scores, key=lambda value: value % 10))
