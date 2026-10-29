# 统计函数：均值与三项摘要。
def mean(values):
    return sum(values) / len(values)


def summarize(values):
    return len(values), round(mean(values), 2), max(values)
