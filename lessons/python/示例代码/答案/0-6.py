# 练习 0-6：改正函数名拼写错误，计算平均分并保留两位小数。
scores = [88, 92, 79]


def average(values):
    return sum(values) / len(values)


print("average:", round(average(scores), 2))
